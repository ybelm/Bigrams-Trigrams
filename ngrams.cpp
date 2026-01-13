#include "ngrams.h"
#include <string>

// Word bigrams
void count_word_bigrams(const std::vector<std::string>& words, WordMap& map) {
    if (words.size() < 2) return;
    for (size_t i = 0; i + 1 < words.size(); ++i) {
        std::string key = words[i] + " " + words[i + 1];
        ++map[key];
    }
}

// Word trigrams
void count_word_trigrams(const std::vector<std::string>& words, WordMap& map) {
    if (words.size() < 3) return;
    for (size_t i = 0; i + 2 < words.size(); ++i) {
        std::string key = words[i] + " " + words[i + 1] + " " + words[i + 2];
        ++map[key];
    }
}

// Char bigrams
void count_char_bigrams(const std::vector<char>& chars, WordMap& map) {
    if (chars.size() < 2) return;
    std::string key(2, '\0');
    for (size_t i = 0; i + 1 < chars.size(); ++i) {
        key[0] = chars[i];
        key[1] = chars[i + 1];
        ++map[key];
    }
}

// Char trigrams
void count_char_trigrams(const std::vector<char>& chars, WordMap& map) {
    if (chars.size() < 3) return;
    std::string key(3, '\0');
    for (size_t i = 0; i + 2 < chars.size(); ++i) {
        key[0] = chars[i];
        key[1] = chars[i + 1];
        key[2] = chars[i + 2];
        ++map[key];
    }
}
