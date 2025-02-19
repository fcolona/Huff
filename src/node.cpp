#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <queue>
#include "../includes/node.hpp"

Node::Node(const char label, const unsigned int freq) 
    : label(label), freq(freq){};

Node::Node(const char label, const unsigned int freq, Node *left, Node *right) 
    : label(label), freq(freq), left(left), right(right){};


struct Compare {
public:
    bool operator()(const Node* a, const Node* b) const {
        if(a->freq > b->freq) return true;
        return false;
    }
};
Node *Node::build_tree(std::map<char, unsigned int> &freq){
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

void Node::serialize_subtree_aux(Node *head, boost::dynamic_bitset<> &bits){
    if(!head->left && !head->right){
        bits.push_back(1);
        for(int i = 7; i >= 0; i--){
            int bit = (head->label >> (7 - i)) & 1;               
            bits.push_back(bit);
        }
        return;
    }
    
    bits.push_back(0);
    serialize_subtree_aux(head->left, bits);
    serialize_subtree_aux(head->right, bits);
}

boost::dynamic_bitset<> Node::serialize_subtree(){
    boost::dynamic_bitset<> bits;
    serialize_subtree_aux(this, bits);
    
    return bits;
}