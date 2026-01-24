#include "stats.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

long long compute_total(const WordMap& map) {
    long long total = 0;
    for (const auto& [k, v] : map) {
        total += v;
    }
    return total;
}

std::vector<std::pair<std::string, long long>> top_n(const WordMap& map, size_t n) {
    std::vector<std::pair<std::string, long long>> vec(map.begin(), map.end());

    size_t k = std::min(n, vec.size());

    std::partial_sort(vec.begin(), vec.begin() + k, vec.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    vec.resize(k);
    return vec;
}

void print_top(const WordMap& map, size_t n, const std::string& label, long long total) {
    auto top = top_n(map, n);

    std::cout << "\n[-- Top " << n << " " << label << " --]\n";

    for (size_t i = 0; i < top.size(); ++i) {
        double pct = (total > 0) ? (100.0 * top[i].second / total) : 0.0;

        std::cout << std::setw(4) << (i + 1) << ".  "
                  << std::setw(30) << std::left << ("\"" + top[i].first + "\"")
                  << std::right << std::setw(10) << top[i].second
                  << "  (" << std::fixed << std::setprecision(3) << pct << "%)\n";
    }
}

void print_stats(const WordMap& map, const std::string& label, size_t top) {
    long long total = compute_total(map);

    std::cout << "\n----------------------------------------\n";
    std::cout << "  " << label << "\n";
    std::cout << "  Unique n-grams: " << map.size() << "\n";
    std::cout << "  Total count: " << total << "\n";
    std::cout << "------------------------------------------\n";

    print_top(map, top, label, total);
}