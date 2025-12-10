#include "preprocessing.h"
#include <cctype>
#include <string>

std::string normalize(const std::string& text) {
    std::string result;
    result.reserve(text.size());

    for (unsigned char c : text) {
        if (std::isalpha(c)) {
            result.push_back(static_cast<char>(std::tolower(c)));
        } else if (std::isspace(c)) {
            // Collapse any whitespace to a single space
            if (!result.empty() && result.back() != ' ') {
                result.push_back(' ');
            }
        }
        // Punctuation and digits are dropped
    }

    // Trim trailing space
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }

    return result;
}
