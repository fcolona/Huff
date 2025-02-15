#include "node.hpp"

Node::Node(const char label, const unsigned int freq) 
    : label(label), freq(freq){};

Node::Node(const char label, const unsigned int freq, Node *left, Node *right) 
    : label(label), freq(freq), left(left), right(right){};