#include "ngrams.h"
#include <string>

// Word bigrams
void count_word_bigrams(const std::vector<std::string>& words, WordMap& map) {
    if (words.size() < 2) return;
    for (size_t i = 0; i + 1 < words.size(); ++i) {
        std::string key = words[i];
        key += " ";
        key += words[i + 1];
        map[key] = map[key] + 1;
    }
}

// Word trigrams
void count_word_trigrams(const std::vector<std::string>& words, WordMap& map) {
    if (words.size() < 3) return;
    for (size_t i = 0; i + 2 < words.size(); ++i) {
        std::string key = words[i];
        key += " ";
        key += words[i + 1];
        key += " ";
        key += words[i + 2];
        map[key] = map[key] + 1;
    }
}

// Char bigrams
void count_char_bigrams(const std::vector<char>& chars, WordMap& map) {
    if (chars.size() < 2) return;
    std::string key = "";
    for (size_t i = 0; i + 1 < chars.size(); ++i) {
        key = "";
        key += chars[i];
        key += chars[i + 1];
        map[key] = map[key] + 1;
    }
}

// Char trigrams
void count_char_trigrams(const std::vector<char>& chars, WordMap& map) {
    if (chars.size() < 3) return;
    std::string key = "";
    for (size_t i = 0; i + 2 < chars.size(); ++i) {
        key = "";
        key += chars[i];
        key += chars[i + 1];
        key += chars[i + 2];
        map[key] = map[key] + 1;
    }
}