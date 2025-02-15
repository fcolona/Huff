#include "node.hpp"

Node::Node(char label, unsigned int freq) 
    : label(label), freq(freq){};

Node::Node(char label, unsigned int freq, Node *left, Node *right) 
    : label(label), freq(freq), left(left), right(right){};