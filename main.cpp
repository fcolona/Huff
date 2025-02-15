#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>

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

void print_frequencies(std::map<char, unsigned int> &frequencies){
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

int main(){
    std::ifstream file;
    file.open("test.txt");
    if(!file.is_open()) throw std::invalid_argument("Could not open file");
    
    std::map<char, unsigned int> frequencies = build_frequencies_map(file);
    print_frequencies(frequencies);
    
    file.close();
    return 0;
}