#include "node.hpp"
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <stdexcept>
#include <boost/dynamic_bitset.hpp>

std::map<char, unsigned int> build_frequencies_map(std::ifstream &file){
    std::map<char, unsigned int> frequencies;
    char ch;
    int i = 0;
    while(file.get(ch)){
        frequencies[ch]++;
        i++;
    }
    std::cout << "Original file size: " << i * sizeof(char) << " bytes" << '\n';
    return frequencies;
}

void print_frequencies(const std::map<char, unsigned int> &frequencies){
    for(auto it = frequencies.begin(); it != frequencies.end(); it++){
        if(it->first == ' '){
            std::cout << "Space";
        } else if (it->first == '\n'){
            std::cout << "NL";
        } else {
            std::cout << it->first;
        }
        std::cout << ": " << it->second << '\n';
    }
}

void print_encodings(const std::map<char, boost::dynamic_bitset<>> &encodings){
    for(auto it = encodings.begin(); it != encodings.end(); it++){
        if(it->first == ' '){
            std::cout << "Space: ";
        } else if (it->first == '\n'){
            std::cout << "NL: ";
        } else {
            std::cout << it->first << ": ";
        }
        
        for(int i = 0; i < it->second.size(); i++){
            std::cout << it->second[i];
        } 
        std::cout << '\n';
    }
}

class Compare {
public:
    bool operator()(const Node* a, const Node* b) const {
        if(a->freq > b->freq) return true;
        return false;
    }
};
Node *build_tree(std::map<char, unsigned int> &freq){
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for(auto it = freq.begin(); it != freq.end(); it++){
        Node* new_node = new Node(it->first, it->second);
        pq.push(new_node);
    }
    
    while(pq.size() > 1){
        Node* n1 = pq.top();
        pq.pop();
        Node* n2 = pq.top();
        pq.pop();
    
        Node* combined = new Node('\0', n1->freq + n2->freq, n1, n2);
        pq.push(combined);
    }
    return pq.top();
}

void traverse_tree(Node *head, std::map<char, boost::dynamic_bitset<>> &encodings, boost::dynamic_bitset<> &current_code){
    if(head->left == NULL && head->right == NULL){
        encodings[head->label] = current_code;
        return;       
    }
    
    if(head->left){
        current_code.push_back(0);
        traverse_tree(head->left, encodings, current_code);       
        current_code.resize(current_code.size()-1);
    }
    if(head->right){
        current_code.push_back(1);
        traverse_tree(head->right, encodings, current_code);
        current_code.resize(current_code.size()-1);
    }
}

std::map<char, boost::dynamic_bitset<>> build_encoding_map(Node *tree_head){
    std::map<char, boost::dynamic_bitset<>> encoding_map;
    boost::dynamic_bitset<> current_code;
    traverse_tree(tree_head, encoding_map, current_code);
    
    return encoding_map;
}

void encode_file(std::ifstream &file, std::map<char, boost::dynamic_bitset<>> &encodings){
    std::ofstream outfile("compressed_file", std::ios::binary);
    if(!outfile.is_open()) throw std::runtime_error("Could not create file");

    file.clear();
    file.seekg(0, std::ios::beg);

    char ch;
    char byte = 0;
    int bit_index = 0;
    int byte_counter = 0;
    while(file.get(ch)){
        for(int j = 0; j < encodings[ch].size(); j++){
            if(bit_index == 8){
                outfile.put(byte);
                byte = 0;
                bit_index = 0;
                byte_counter++;
            }
            if(encodings[ch][j]){
                byte |= (1 << (7 - bit_index));
            }
            bit_index++;
        }
    }
    if (bit_index > 0){
        outfile.put(byte);
        byte_counter++;
    }
    std::cout << "Compressed file size: " << byte_counter << " bytes" << '\n';
    outfile.close();
}

std::string decode_file(std::ifstream &file, Node *tree_head){
    Node *current = tree_head;
    file.clear();
    file.seekg(0, std::ios::beg);

    char byte;
    std::string decoded_txt;
    while(file.get(byte)){
        for(int i = 7; i >= 0; i--){
            if(!current->left && !current->right){
                decoded_txt.push_back(current->label);
                current = tree_head;
            }

            bool bit = (byte >> i) & 1;
            if(!bit && tree_head->left){
                current = current->left;
            } else {
                current = current->right;
            }
        }
    }
    return decoded_txt;
}

int main(){
    std::ifstream file;
    file.open("test.txt");
    if(!file.is_open()) throw std::invalid_argument("Could not open file");
    
    std::map<char, unsigned int> frequencies = build_frequencies_map(file);
    std::cout << "Frequency map: \n";
    print_frequencies(frequencies);
    
    Node* tree_head = build_tree(frequencies);
    std::map<char, boost::dynamic_bitset<>> encodings = build_encoding_map(tree_head);
    std::cout << "Encoding map: \n";
    print_encodings(encodings);
    
    encode_file(file, encodings);
    
    std::ifstream compressed_file;
    compressed_file.open("compressed_file");
    if(!compressed_file.is_open()) throw std::invalid_argument("Could not open file");
    std::string decoded_txt = decode_file(compressed_file, tree_head);
    
    std::cout << "Decoded text: \n" << decoded_txt << '\n';
    file.close();
    return 0;
}
