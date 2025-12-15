#ifndef NGRAMS_H
#define NGRAMS_H

#include <vector>
#include <string>
#include <unordered_map>

// Type alias for frequency maps
using WordMap = std::unordered_map<std::string, long long>;

// Word n-grams
void count_word_bigrams(const std::vector<std::string>& words, WordMap& map);
void count_word_trigrams(const std::vector<std::string>& words, WordMap& map);

// Character n-grams
void count_char_bigrams(const std::vector<char>& chars, WordMap& map);
void count_char_trigrams(const std::vector<char>& chars, WordMap& map);

#endif