#pragma once
class Node {
public:
    char label;
    unsigned int freq;
    Node *left;
    Node *right;
    
    Node(const char label, const unsigned int freq);
    Node(const char label, const unsigned int freq, Node *left, Node *right);
};