#pragma once

#include "ngrams.h"  // WordMap
#include <omp.h>


void count_word_bigrams_parallel (const std::vector<std::string>& words, WordMap& map,
                                  omp_sched_t sched = omp_sched_static, int chunk = 0);
void count_word_trigrams_parallel(const std::vector<std::string>& words, WordMap& map,
                                  omp_sched_t sched = omp_sched_static, int chunk = 0);
void count_char_bigrams_parallel (const std::vector<char>& chars,        WordMap& map,
                                  omp_sched_t sched = omp_sched_static, int chunk = 0);
void count_char_trigrams_parallel(const std::vector<char>& chars,        WordMap& map,
                                  omp_sched_t sched = omp_sched_static, int chunk = 0);