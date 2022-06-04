CXX=nvcc
CXXFLAGS=-std=c++14 -O2 -o a3

all: a3

a3: a3.cu a3.hpp
	$(CXX) $(CXXFLAGS) a3.cu

clean:
	rm -f a3
