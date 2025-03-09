#pragma once
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <fstream>
#include <map>

class Node {
public:
    char label;
    unsigned int freq;
    Node *left;
    Node *right;
    
    Node();
    Node(const char label);
    Node(const char label, const unsigned int freq);
    Node(const char label, const unsigned int freq, Node *left, Node *right);
    
    static Node *build_tree(std::map<char, unsigned int> const &freq);
    //last_bit_pos is an output parameter
    //it will store the last read bit position + 1 
    static Node *deserialize_tree(std::ifstream &file, unsigned int &last_bit_pos);
    boost::dynamic_bitset<> serialize_subtree();

private:
    void serialize_subtree_aux(const Node *head, boost::dynamic_bitset<> &bits);
    static Node *deserialize_tree_aux(std::ifstream &file, unsigned int &bit_pos);
};