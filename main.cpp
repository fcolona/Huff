#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>

std::map<char, int> build_frequencies_map(std::ifstream &file){
    std::map<char, int> frequencies;
    char ch;
    while(file.get(ch)){
        frequencies[ch]++;
    }
    return frequencies;
}

void print_frequencies(std::map<char, int> &frequencies){
    std::map<char, int>::iterator it; 
    for(it = frequencies.begin(); it != frequencies.end(); it++){
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
    
    std::map<char, int> frequencies = build_frequencies_map(file);
    print_frequencies(frequencies);

    file.close();
    return 0;
}