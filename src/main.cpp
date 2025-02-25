#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <boost/dynamic_bitset.hpp>
#include "../includes/node.hpp"
#include "../includes/frequencies.hpp"
#include "../includes/encodings.hpp"

#define HEADER_SIZE 4

void encode_file(std::ifstream &file, boost::dynamic_bitset<> &tree_serialization, std::map<char, boost::dynamic_bitset<>> &encodings){
    std::ofstream outfile("compressed_file", std::ios::binary);
    if(!outfile.is_open()) throw std::runtime_error("Could not create file");

    file.clear();
    file.seekg(0, std::ios::beg);

    //Write header
    char ch;
    int bit_counter = 0;
    while(file.get(ch)){
        bit_counter += encodings[ch].size();        
    }
    bit_counter += tree_serialization.size();
    bit_counter += 8 * HEADER_SIZE;
    outfile.write(reinterpret_cast<const char *>(&bit_counter), sizeof(bit_counter));
    
    //Write serialized tree
    char byte = 0;
    int bit_index = 0;
    int message_byte_counter = 0;
    for(int i = 0; i < tree_serialization.size(); i++){
        if(bit_index == 8){
            outfile.put(byte);
            byte = 0;
            bit_index = 0;
            message_byte_counter++;
        }
        if(tree_serialization[i]){
            byte |= (1 << (7 - bit_index));
        }
        bit_index++;
    }

    file.clear();
    file.seekg(0, std::ios::beg);
    
    //Write encoded text content
    while(file.get(ch)){
        for(int j = 0; j < encodings[ch].size(); j++){
            if(bit_index == 8){
                outfile.put(byte);
                byte = 0;
                bit_index = 0;
                message_byte_counter++;
            }
            if(encodings[ch][j]){
                byte |= (1 << (7 - bit_index));
            }
            bit_index++;
        }
    }
    if (bit_index > 0){
        outfile.put(byte);
        message_byte_counter++;
    }
    std::cout << "Compressed file size: " << message_byte_counter + HEADER_SIZE << " bytes" << '\n';
    outfile.close();
}

std::string decode_file(std::ifstream &file, Node *tree_head, unsigned int start_of_content){
    Node *current = tree_head;
    file.clear();
    file.seekg(0, std::ios::beg);

    int end_of_content;
    char byte;
    char header_bytes[HEADER_SIZE];
    for(int i = 0; i < HEADER_SIZE; i++){
        file.get(byte);
        header_bytes[i] = byte;
    }
    std::memcpy(&end_of_content, header_bytes, HEADER_SIZE);

    int ini;
    if(start_of_content % 8 == 0){
    //when start_of_content is the last bit of a byte
        file.seekg(start_of_content/8 - 1, std::ios::beg);
        ini = 7;
    } else {
        file.seekg(start_of_content/8, std::ios::beg);
        ini = start_of_content%8;
    }

    std::string decoded_txt;
    int bit_counter = start_of_content;
    while(file.get(byte)){
        for(int i = 7 - ini; i >= -1; i--){
            if(bit_counter > end_of_content) return decoded_txt;

            bool bit = (byte >> (i+1)) & 1;
            if(!bit && tree_head->left){
                current = current->left;
            } else {
                current = current->right;
            }

            if(!current->left && !current->right){
                decoded_txt.push_back(current->label);
                current = tree_head;
            }
            bit_counter++;
        }
        ini = 1;
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
    
    boost::dynamic_bitset<> tree_serialization = tree_head->serialize_subtree();
    std::cout << "Tree serialization: " << tree_serialization << std::endl;
    
    encode_file(file, tree_serialization, encodings);
    
    std::ifstream compressed_file;
    compressed_file.open("compressed_file");
    if(!compressed_file.is_open()) throw std::invalid_argument("Could not open file");

    unsigned int last_bit_pos;
    tree_head = Node::deserialize_tree(compressed_file, last_bit_pos);
    std::string decoded_txt = decode_file(compressed_file, tree_head, last_bit_pos);
    
    std::cout << "Decoded text: \n" << decoded_txt << '\n';
    file.close();
    return 0;
}
