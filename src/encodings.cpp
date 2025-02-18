#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <iostream>
#include <map>
#include "../includes/node.hpp"
#include "../includes/encodings.hpp"

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

void populate_encodings(Node *head, std::map<char, boost::dynamic_bitset<>> &encodings, boost::dynamic_bitset<> &current_code){
    if(head->left == NULL && head->right == NULL){
        encodings[head->label] = current_code;
        return;       
    }
    
    if(head->left){
        current_code.push_back(0);
        populate_encodings(head->left, encodings, current_code);       
        current_code.resize(current_code.size()-1);
    }
    if(head->right){
        current_code.push_back(1);
        populate_encodings(head->right, encodings, current_code);
        current_code.resize(current_code.size()-1);
    }
}

std::map<char, boost::dynamic_bitset<>> build_encoding_map(Node *tree_head){
    std::map<char, boost::dynamic_bitset<>> encoding_map;
    boost::dynamic_bitset<> current_code;
    populate_encodings(tree_head, encoding_map, current_code);
    
    return encoding_map;
}