#include "tokenizer.h"
#include <sstream>

std::vector<std::string> tokenize_words(const std::string& text) {
    std::vector<std::string> words;
    std::istringstream stream(text);
    std::string word;
    while (stream >> word) {
        words.push_back(std::move(word));
    }
    return words;
}

std::vector<char> tokenize_chars(const std::string& text) {
    std::vector<char> chars;
    chars.reserve(text.size());
    for (char c : text) {
        if (c >= 'a' && c <= 'z') {
            chars.push_back(c);
        }
    }
    return chars;
}
