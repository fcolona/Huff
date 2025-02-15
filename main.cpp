#include "node.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
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

class Compare {
public:
    bool operator()(const Node* a, const Node* b) const {
        if(a->freq > b->freq) return true;
        return false;
    }
};
Node* build_tree(std::map<char, unsigned int> &freq){
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

int main(){
    std::ifstream file;
    file.open("test.txt");
    if(!file.is_open()) throw std::invalid_argument("Could not open file");
    
    std::map<char, unsigned int> frequencies = build_frequencies_map(file);
    print_frequencies(frequencies);
    
    Node* head = build_tree(frequencies);
    
    file.close();
    return 0;
}