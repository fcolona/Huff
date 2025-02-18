#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <boost/dynamic_bitset.hpp>
#include "node.hpp"
#include "frequencies.hpp"
#include "encodings.hpp"

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
    
    Node* tree_head = Node::build_tree(frequencies);
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
