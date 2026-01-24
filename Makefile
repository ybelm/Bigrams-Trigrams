# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -O3 -march=native -fopenmp -Isrc

TARGET := ngram
SRCS := $(wildcard src/*.cpp)

# Build
all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Remove compiled binary
clean:
	rm -f $(TARGET)

.PHONY: all clean