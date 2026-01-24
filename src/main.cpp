#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

#include <omp.h>

#include "preprocessing.h"
#include "tokenizer.h"
#include "ngrams.h"
#include "ngrams_parallel.h"
#include "stats.h"


// Helpers
static std::string read_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static std::string build_corpus(const std::string& base, int repetitions) {
    std::string corpus;
    corpus.reserve(base.size() * static_cast<size_t>(repetitions));
    for (int i = 0; i < repetitions; ++i) {
        corpus += base;
        corpus += '\n';
    }
    return corpus;
}

// Verify two WordMaps are identical (used to validate parallel output)
static bool maps_equal(const WordMap& a, const WordMap& b) {
    if (a.size() != b.size()) return false;
    for (const auto& [k, v] : a) {
        auto it = b.find(k);
        if (it == b.end() || it->second != v) return false;
    }
    return true;
}

// Run fn() N times
template<typename Fn>
static std::vector<double> benchmark(Fn fn, int runs) {
    std::vector<double> times;
    times.reserve(runs);
    for (int r = 0; r < runs; ++r) {
        double t0 = omp_get_wtime();
        fn();
        times.push_back(omp_get_wtime() - t0);
    }
    return times;
}

static void print_bench(const std::string& label, const std::vector<double>& times) {
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();
    double sq = 0.0;
    for (double t : times) sq += (t - mean) * (t - mean);
    double std_ = std::sqrt(sq / times.size());
    double mn = *std::min_element(times.begin(), times.end());
    double mx = *std::max_element(times.begin(), times.end());

    std::cout << label
              << " mean=" << mean << "s"
              << " std=" << std_ << "s"
              << " min=" << mn << "s"
              << " max=" << mx << "s\n";
}

static omp_sched_t parse_schedule(const std::string& s) {
    if (s == "dynamic") return omp_sched_dynamic;
    if (s == "guided")  return omp_sched_guided;
    if (s == "auto") return omp_sched_auto;
    return omp_sched_static; // default
}

/*
 * Usage:
 *   ./ngram <input_file> [repetitions=100] [threads=max] [runs=5] [mode=both] [sched=static] [chunk=0]
 *
 *   mode:  seq | par | both
 *   sched: static | dynamic | guided | auto
 *   chunk: 0 = OpenMP decides automatically
 *
 * Examples:
 *   ./ngram input.txt 100 8 5 both static 0
 *   ./ngram input.txt 100 8 5 both dynamic 512
 *   ./ngram input.txt 100 8 5 par guided 0
 */
int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <input_file> [repetitions=100] [threads=max] [runs=5] [mode=both] [sched=static] [chunk=0]\n";
        return 1;
    }

    const std::string input_path = argv[1];
    const int repetitions = (argc >= 3) ? std::stoi(argv[2]) : 100;
    const int req_threads = (argc >= 4) ? std::stoi(argv[3]) : omp_get_max_threads();
    const int runs = (argc >= 5) ? std::stoi(argv[4]) : 5;
    const std::string mode = (argc >= 6) ? std::string(argv[5]) : "both";
    const std::string  sched_str = (argc >= 7) ? std::string(argv[6]) : "static";
    const int chunk_size = (argc >= 8) ? std::stoi(argv[7]) : 0;

    const omp_sched_t sched_kind = parse_schedule(sched_str);

    // Apply requested thread count
    omp_set_num_threads(req_threads);

    // System info
    std::cout << "\n[System Info]\n";
    std::cout << "Requested threads: " << req_threads << "\n";
    std::cout << "Benchmark runs: " << runs << "\n";
    std::cout << "Mode: " << mode << "\n";
    std::cout << "Schedule: " << sched_str << "\n";
    std::cout << "Chunk size: " << chunk_size << " (0 = auto)\n";

    // Load & build corpus (excluded from timing)
    std::cout << "\n[1/3] Loading & building corpus...\n";
    std::string base_text = read_file(input_path);
    std::cout << "Base file size: " << base_text.size() / 1024 << " KB\n";

    std::string corpus = build_corpus(base_text, repetitions);
    std::cout << "Corpus size: " << corpus.size() / (1024 * 1024) << " MB\n";

    // Preprocessing & tokenization (sequential, excluded from n-gram timing)
    std::cout << "\n[2/3] Preprocessing & tokenizing...\n";
    double t0 = omp_get_wtime();

    std::string normalized = normalize(corpus);
    std::vector<std::string> words = tokenize_words(normalized);
    std::vector<char> chars = tokenize_chars(normalized);

    std::cout << "Words: " << words.size() << "\n";
    std::cout << "Chars: " << chars.size() << "\n";
    std::cout << "Time: " << omp_get_wtime() - t0 << " s\n";

    // N-gram counting
    std::cout << "\n[3/3] Counting n-grams (mode=" << mode << ", " << runs << " runs)...\n\n";

    // Warm-up run (discarded) to avoid cold-cache effects
    {
        WordMap tmp;
        count_word_bigrams(words, tmp);
        count_word_trigrams(words, tmp);
        count_char_bigrams(chars, tmp);
        count_char_trigrams(chars, tmp);
    }

    double seq_best = -1.0;
    double par_best = -1.0;

    // Sequential
    WordMap seq_wb, seq_wt, seq_cb, seq_ct;

    if (mode == "seq" || mode == "both") {
        std::cout << "-- SEQUENTIAL --\n";

        auto run_seq = [&]() {
            seq_wb.clear(); seq_wt.clear();
            seq_cb.clear(); seq_ct.clear();
            count_word_bigrams (words, seq_wb);
            count_word_trigrams(words, seq_wt);
            count_char_bigrams (chars, seq_cb);
            count_char_trigrams(chars, seq_ct);
        };

        auto times = benchmark(run_seq, runs);
        seq_best = *std::min_element(times.begin(), times.end()); // use best time
        print_bench("Sequential n-gram count", times);
        std::cout << "\n";
    }

    // Parallel
    WordMap par_wb, par_wt, par_cb, par_ct;

    if (mode == "par" || mode == "both") {
        std::cout << "-- PARALLEL (threads=" << req_threads << ", sched=" << sched_str 
        << ", chunk=" << chunk_size << ") --\n";

        auto run_par = [&]() {
            par_wb.clear(); par_wt.clear();
            par_cb.clear(); par_ct.clear();
            count_word_bigrams_parallel (words, par_wb, sched_kind, chunk_size);
            count_word_trigrams_parallel(words, par_wt, sched_kind, chunk_size);
            count_char_bigrams_parallel(chars, par_cb, sched_kind, chunk_size);
            count_char_trigrams_parallel(chars, par_ct, sched_kind, chunk_size);
        };

        auto times = benchmark(run_par, runs);
        par_best = *std::min_element(times.begin(), times.end());
        print_bench("Parallel n-gram count", times);
        std::cout << "\n";
    }

    // Correctness check
    if (mode == "both") {
        std::cout << "Correctness check \n";
        std::cout << "Word bigrams: " << (maps_equal(seq_wb, par_wb) ? "OK" : "MISMATCH") << "\n";
        std::cout << "Word trigrams: " << (maps_equal(seq_wt, par_wt) ? "OK" : "MISMATCH") << "\n";
        std::cout << "Char bigrams: " << (maps_equal(seq_cb, par_cb) ? "OK" : "MISMATCH") << "\n";
        std::cout << "Char trigrams: " << (maps_equal(seq_ct, par_ct) ? "OK" : "MISMATCH") << "\n";
        std::cout << "\n";
    }

    // Speedup
    if (mode == "both" && seq_best > 0 && par_best > 0) {
        double speedup = seq_best / par_best;
        double efficiency = speedup / req_threads * 100.0;

        std::cout << "Speedup \n";
        std::cout << "T_seq (best): " << seq_best << " s\n";
        std::cout << "T_par (best): " << par_best << " s\n";
        std::cout << "Speedup: " << speedup << "x\n";
        std::cout << "Efficiency: " << efficiency << "%\n";
        std::cout << "\n";
    }

    // Stats (printed once, from sequential or parallel result)
    const WordMap& wb = (mode == "par") ? par_wb : seq_wb;
    const WordMap& wt = (mode == "par") ? par_wt : seq_wt;
    const WordMap& cb = (mode == "par") ? par_cb : seq_cb;
    const WordMap& ct = (mode == "par") ? par_ct : seq_ct;

    print_stats(wb, "Word Bigrams");
    print_stats(wt, "Word Trigrams");
    print_stats(cb, "Char Bigrams");
    print_stats(ct, "Char Trigrams");

    return 0;
}