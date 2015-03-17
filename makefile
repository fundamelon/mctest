# Makefile for MCFlow Test Utilities
# Author: Igi Chorazewicz
# Date: December 20, 2014
# Description: 
#
#
# Usage:
#	Super rudimentary makefile
#

FLAGS = -W -Wall -pedantic -ansi -std=c++11

mctest.exe: main.o util.o Benchmark.o
	g++ $(FLAGS) -o mctest main.o util.o Benchmark.o

main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

util.o: util.cpp
	g++ $(FLAGS) -c util.cpp
	
Benchmark.o: Benchmark.cpp
	g++ $(FLAGS) -c Benchmark.cpp
	
test:
	make
	./mctest -A -B

clean: 
	rm *.o mctest
