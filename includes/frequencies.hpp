#pragma once
#include <fstream>
#include <map>

std::map<char, unsigned int> build_frequencies_map(std::ifstream &file);
void print_frequencies(const std::map<char, unsigned int> &frequencies);