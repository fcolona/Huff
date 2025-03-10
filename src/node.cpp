#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <fstream>
#include <queue>
#include "../includes/node.hpp"

#define HEADER_SIZE 4

Node::Node()
    : label('\0'), freq(0), left(nullptr), right(nullptr){}

Node::Node(const char label) 
    : label(label), freq(0), left(nullptr), right(nullptr){};

Node::Node(const char label, const unsigned int freq) 
    : label(label), freq(freq), left(nullptr), right(nullptr){};

Node::Node(const char label, const unsigned int freq, Node *left, Node *right) 
    : label(label), freq(freq), left(left), right(right){};


struct Compare {
public:
    bool operator()(const Node* a, const Node* b) const {
        if(a->freq > b->freq) return true;
        return false;
    }
};
Node *Node::build_tree(const std::map<char, unsigned int> &freq){
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

void Node::delete_tree(Node *head){
    if(!head->left && !head->right){
        delete head;
        return;
    }

    delete_tree(head->left);
    delete_tree(head->right);
    delete head;
}

void Node::serialize_subtree_aux(const Node *head, boost::dynamic_bitset<> &bits){
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

Node *Node::deserialize_tree_aux(std::ifstream &file, unsigned int &bit_pos){
    int current_byte_index;
    bool is_last_bit = false;
    if(bit_pos % 8 != 0 && bit_pos != 0) current_byte_index = bit_pos/8;       
    else {
        current_byte_index = bit_pos/8 - 1;   
        is_last_bit = true;
    }
    file.seekg(current_byte_index, std::ios::beg);
    
    char byte;
    file.get(byte);
    bool bit;
    if(bit_pos % 8 != 0) bit = (byte >> (7 - ( (bit_pos % 8) - 1)) ) & 1;
    else bit = byte & 1; 
    
    if(is_last_bit) file.get(byte);
    Node *head = new Node();
    if(!bit){
    //internal node
        head->label = '\0';
        
        head->left = deserialize_tree_aux(file, ++bit_pos);
        head->right = deserialize_tree_aux(file, bit_pos);
    } else {
    //leaf node
        char label = '\0';
        bit_pos++;
        for(int i = 0; i < 8; i++){
        //collect the bits representing the node label
            if(bit_pos % 8 == 0){
            //has reached the last bit of the current byte
                bool bit = byte & 1;
                label |= (bit << i);
                file.get(byte);
                bit_pos++;
                continue;
            }
            bool bit = (byte >> (8 - (bit_pos%8))) & 1;
            label |= (bit << i);
            bit_pos++;
        }
        head->label = label;
    }
    return head;
}

Node *Node::deserialize_tree(std::ifstream &file, unsigned int &last_bit_pos){
    file.clear();
    file.seekg(0, std::ios::beg);

    unsigned int current_bit_pos = 8 * HEADER_SIZE + 1;
    Node *tree_head = deserialize_tree_aux(file, current_bit_pos);
    
    last_bit_pos = current_bit_pos;
    return tree_head;
}