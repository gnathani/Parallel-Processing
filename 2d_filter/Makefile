CXX=g++
CXXFLAGS=-fopenmp -std=c++17 -O2 -o a0

all: a0

a0: a0.cpp a0.hpp
	$(CXX) -std=c++17 -O2 -o bin_seq_r a0.cpp
	$(CXX) $(CXXFLAGS) a0.cpp
clean:
	rm -rf a0
	rm -rf bin_seq_r
