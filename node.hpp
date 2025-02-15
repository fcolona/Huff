#pragma once
class Node {
public:
    char label;
    unsigned int freq;
    Node *left;
    Node *right;
    
    Node(char label, unsigned int freq);
    Node(char label, unsigned int freq, Node *left, Node *right);
};