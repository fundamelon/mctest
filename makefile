# Makefile for MCFlow Test Utilities
# Author: Igi Chorazewicz
# Date: December 20, 2014
# Description: 
#
#
# Usage:
#	Use 'make test' to fully test the program
#

CXX = g++
CXXFLAGS = -W -Wall -pedantic -ansi -std=c++11
BIN = obj

DEPS = util.h Benchmark.h Test.h

OBJ = main.o util.o Benchmark.o Test.o

%.o: %.cpp $(DEPS)
	$(CXX) -MMD -c -o $@ $< $(CXXFLAGS)

mctest: main.o util.o Benchmark.o Test.o
	$(CXX) -o mctest main.o util.o Benchmark.o Test.o $(CXXFLAGS) 

	
test:
	make
	./mctest -A -B

.PHONY: clean
clean: 
	rm -f *.o *~ mctest
	rm obj
