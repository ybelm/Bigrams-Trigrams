#include "ngrams_parallel.h"
#include <omp.h>
#include <vector>
#include <string>

// Merge all thread-local maps into the global map (sequential, called after parallel region)
static void merge_maps(std::vector<WordMap>& local_maps, WordMap& global_map) {
    for (auto& lmap : local_maps) {
        for (auto it = lmap.begin(); it != lmap.end(); ++it) {
            const std::string& key = it->first;
            int count = it->second;
            global_map[key] = global_map[key] + count;
        }
    }
}

void count_word_bigrams_parallel(const std::vector<std::string>& words, WordMap& global_map, omp_sched_t sched, int chunk) {
    if (words.size() < 2) return;
    omp_set_schedule(sched, chunk);

    std::vector<WordMap> local_maps(omp_get_max_threads());

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        WordMap& local = local_maps[tid];

        #pragma omp for schedule(runtime)
        for (long long i = 0; i < (long long)words.size() - 1; ++i) {
            std::string key = words[i];
            key += " ";
            key += words[i + 1];
            local[key] = local[key] + 1;
        }
    }

    merge_maps(local_maps, global_map);
}

void count_word_trigrams_parallel(const std::vector<std::string>& words, WordMap& global_map, omp_sched_t sched, int chunk) {
    if (words.size() < 3) return;
    omp_set_schedule(sched, chunk);

    std::vector<WordMap> local_maps(omp_get_max_threads());

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        WordMap& local = local_maps[tid];

        #pragma omp for schedule(runtime)
        for (long long i = 0; i < (long long)words.size() - 2; ++i) {
            std::string key = words[i];
            key += " ";
            key += words[i + 1];
            key += " ";
            key += words[i + 2];
            local[key] = local[key] + 1;
        }
    }

    merge_maps(local_maps, global_map);
}

void count_char_bigrams_parallel(const std::vector<char>& chars, WordMap& global_map, omp_sched_t sched, int chunk) {
    if (chars.size() < 2) return;
    omp_set_schedule(sched, chunk);

    std::vector<WordMap> local_maps(omp_get_max_threads());

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        WordMap& local = local_maps[tid];

        #pragma omp for schedule(runtime)
        for (long long i = 0; i < (long long)chars.size() - 1; ++i) {
            std::string key = "";
            key += chars[i];
            key += chars[i + 1];
            local[key] = local[key] + 1;
        }
    }

    merge_maps(local_maps, global_map);
}

void count_char_trigrams_parallel(const std::vector<char>& chars, WordMap& global_map, omp_sched_t sched, int chunk) {
    if (chars.size() < 3) return;
    omp_set_schedule(sched, chunk);

    std::vector<WordMap> local_maps(omp_get_max_threads());

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        WordMap& local = local_maps[tid];

        #pragma omp for schedule(runtime)
        for (long long i = 0; i < (long long)chars.size() - 2; ++i) {
            std::string key = "";
            key += chars[i];
            key += chars[i + 1];
            key += chars[i + 2];
            local[key] = local[key] + 1;
        }
    }

    merge_maps(local_maps, global_map);
}