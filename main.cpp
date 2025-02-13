#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

int main(){
    std::ifstream file;
    file.open("test.txt");
    if(!file.is_open()) throw std::invalid_argument("Could not open file");
    
    std::string line;
    while(std::getline(file, line)){
        std::cout << line << "\n";
    }
    file.close();

    return 0;
}