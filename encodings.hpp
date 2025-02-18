#pragma once
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <map>
#include "node.hpp"

void print_encodings(const std::map<char, boost::dynamic_bitset<>> &encodings);
std::map<char, boost::dynamic_bitset<>> build_encoding_map(Node *tree_head);
void populate_encodings(Node *head, std::map<char, boost::dynamic_bitset<>> &encodings, boost::dynamic_bitset<> &current_code);