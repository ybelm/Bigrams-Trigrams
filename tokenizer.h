#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>

// Split text into words
std::vector<std::string> tokenize_words(const std::string& text);

// Extract characters
std::vector<char> tokenize_chars(const std::string& text);

#endif