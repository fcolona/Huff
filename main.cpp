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

int main(){
    std::ifstream file;
    file.open("test.txt");
    if(!file.is_open()) throw std::invalid_argument("Could not open file");
    
    std::map<char, unsigned int> frequencies = build_frequencies_map(file);
    print_frequencies(frequencies);
    
    Node* tree_head = build_tree(frequencies);
    std::map<char, boost::dynamic_bitset<>> encodings = build_encoding_map(tree_head);
    std::cout << "\n";
    print_encodings(encodings);
    
    file.close();
    return 0;
}
