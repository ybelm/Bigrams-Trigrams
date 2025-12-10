#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <string>

// Normalize text (lowercase, remove punctuation and digits, collapse whitespace)
std::string normalize(const std::string& text);

#endif