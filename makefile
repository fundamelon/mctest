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

DEPS = util.h Benchmark.h Test.h

MKDIR_P = mkdir -p
ODIR = bin/

OBJ = $(addprefix $(ODIR), main.o util.o Benchmark.o Test.o )

.PHONY: all
all:
	$(MKDIR_P) $(ODIR)
	make mctest

$(ODIR)%.o: %.cpp
	$(CXX) -MMD -c -o $@ $< $(CXXFLAGS)

mctest: $(OBJ)
	$(CXX) -o mctest $(OBJ) $(CXXFLAGS) 


.PHONY: test
test:
	make
#	clear
	./mctest -A -B

.PHONY: clean
clean: 
	rm -f *.o *.d mctest *~
	rm -rf $(ODIR)
