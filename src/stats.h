#ifndef STATS_H
#define STATS_H

#include <vector>
#include <string>
#include "ngrams.h"

// Return top-N most frequent n-grams
std::vector<std::pair<std::string, long long>>
top_n(const WordMap& map, size_t n);

// Print statistics and top elements
void print_stats(const WordMap& map, const std::string& label, size_t top = 10);

#endif