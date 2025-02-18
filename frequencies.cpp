#include <fstream>
#include <iostream>
#include <map>
#include "frequencies.hpp"

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
