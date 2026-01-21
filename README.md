
# Parallel N-gram Frequency Analysis

Parallel counting of bigrams and trigrams (words and characters) on large text corpora, implemented in C++17 with OpenMP.

## Project structure

```
.
├── preprocessing.h / .cpp      # Text normalization
├── tokenizer.h / .cpp          # Tokenization into words and characters
├── ngrams.h / .cpp             # Sequential counting
├── ngrams_parallel.h / .cpp    # Parallel counting (OpenMP)
├── stats.h / .cpp              # Statistics and output
├── main.cpp                    # Entry point
```

## Dependencies

* GCC >= 11 with C++17 and OpenMP support
* Python >= 3.9 with `matplotlib` and `numpy` (only for benchmarks)

## Compilation

```bash
make
```

## Usage

```
./ngram <input_file> [repetitions] [threads] [runs] [mode] [sched] [chunk]
```

| Parameter     | Default | Possible values                       |
| ------------- | ------- | ------------------------------------- |
| `input_file`  | —       | path to input text file               |
| `repetitions` | 100     | integer > 0                           |
| `threads`     | max     | integer > 0                           |
| `runs`        | 5       | integer > 0                           |
| `mode`        | both    | `seq`, `par`, `both`                  |
| `sched`       | static  | `static`, `dynamic`, `guided`, `auto` |
| `chunk`       | 0       | integer >= 0 (0 = automatic)          |

Example:

```bash
./ngram input.txt 500 8 7 both static 0
```

Runs the counting on a corpus obtained by repeating the input file 500 times, using 8 threads, 7 runs per configuration, both sequential and parallel versions, static scheduling, and automatic chunk size.

## Notes

The first run of each configuration is discarded as warm-up. Reported times include only the n-gram counting phase, excluding preprocessing and tokenization.

