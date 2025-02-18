#pragma once
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <map>

class Node {
public:
    char label;
    unsigned int freq;
    Node *left;
    Node *right;
    
    Node(const char label, const unsigned int freq);
    Node(const char label, const unsigned int freq, Node *left, Node *right);
    
    static Node *build_tree(std::map<char, unsigned int> &freq);
};