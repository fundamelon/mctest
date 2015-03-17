/* filename: main.cpp
 * Author: Igi Chorazewicz
 * Date Created: December 20, 2014
 * Description: test harness entry point
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "util.h"
#include "Benchmark.h"
 
 //Flags 
 // TODO: make these actual flags :(
bool abort_enabled = false;

bool automation_enabled = false;

int benchmark_flags = 0;

bool benchmarks_enabled = false;
bool benchmarks_time = false;


 
 
int main(int argc,char* argv[]) {
	
	//TODO: All COUT are temporary.  Create custom stream that also
	//	redirects to a log file.
	
	std::cout << "\n-- MCFlow Test Harness --\n\n";
	
	util::init();
	
	// parse command line options
	// TODO: custom implementation, not dependant on getopt (allow --A=B syntax)
	int c;
	opterr = 0;
	
	while((c = getopt(argc, argv, "AaB::b::D:d:Hh")) != -1) {
		std::string arg = (optarg != NULL ? optarg : "");
		switch(c) {
				// set to abort on unsuccessful run
				case 'A':
				case 'a':
					abort_enabled = true;
					break;
					
				// set to run benchmarks
				case 'B':
				case 'b':
					benchmark_flags |= BENCHMARK_FLAG::RUN_ALL;
					if(arg == "verb") 		benchmark_flags |= BENCHMARK_FLAG::VERBOSE;
					else if(arg == "save") 	benchmark_flags |= BENCHMARK_FLAG::SAVE;
					else if(arg == "clear") benchmark_flags |= BENCHMARK_FLAG::CLEAR;
					else if(arg != "") { // exit if invalid
						std::cout << "Invalid argument \"" << arg << "\".\n";
						return 1;
					}
					break;
					
				// set directory option (TODO: check if valid)
				case 'D':
				case 'd':
					if(util::main_dir == "") std::cout << "Changing";
					else std::cout << "Setting new";
					std::cout << " directory.\n";
					util::main_dir = optarg;
					util::setProgramDir(util::main_dir);
					break;
					
				// print help
				case 'H':
				case 'h':
					std::cout << util::readFromFile("help.txt");
					return 1;
					
				// unknown option
				case '?':
					if(optopt == '?')
						fprintf(stderr, "Option -%c requires an argument. ", optopt);
					else if(isprint(optopt))
						fprintf(stderr, "Unknown option '-%c'. ", optopt);
					else
						fprintf(stderr, "Unknown option character '\\x%x'. ", optopt);
					
				default:
					
					fprintf(stderr, "Please try <./mctest -h> for usage details.\n\n");
				//	abort();
					return 1;
		}
	}
	
	if(util::main_dir == "") {
		std::cout << "Load from previous directory...\n";
	} else if(util::main_dir[util::main_dir.length()-1] == '\n') {
		util::main_dir.erase(util::main_dir.length()-1); // strip newline
	}

	std::cout << "MCFlow Directory: ";
	std::cout << (util::main_dir == "" ? "(not set)" : util::main_dir) << "\n\n";
	
		
	if(benchmark_flags & BENCHMARK_FLAG::VERBOSE) std::cout << "Benchmarks set to verbose.\n";
	if(benchmark_flags & BENCHMARK_FLAG::SAVE) std::cout << "Benchmarks will record output.\n";
	
	
	//Run benchmarks
	//TODO: Move elsewhere, organize.
	
	if(benchmark_flags & BENCHMARK_FLAG::RUN_ALL) {
	
		std::cout << "Running benchmarks...\n";
		
		std::vector<Benchmark> benchmarks;
		
	//	float start_time = util::getTime(), end_time;
		
		benchmarks.push_back(Benchmark("arch10-1s", "synthetic1_on_arch10-1s"));
		benchmarks.push_back(Benchmark("arch10-2s", "synthetic1_on_arch10-2s"));
		benchmarks.push_back(Benchmark("arch20-1s", "synthetic2_on_arch20-1s"));
		benchmarks.push_back(Benchmark("arch20-2s", "synthetic2_on_arch20-2s"));
		benchmarks.push_back(Benchmark("arch30-1s", "synthetic3_on_arch30-1s"));
		benchmarks.push_back(Benchmark("arch30-2s", "synthetic3_on_arch30-2s"));
		benchmarks.push_back(Benchmark("arch40-1s", "synthetic4_on_arch40-1s"));
		benchmarks.push_back(Benchmark("arch40-2s", "synthetic4_on_arch40-2s"));
		benchmarks.push_back(Benchmark("arch50-1s", "synthetic5_on_arch50-1s"));
		benchmarks.push_back(Benchmark("arch50-2s", "synthetic5_on_arch50-2s"));
		benchmarks.push_back(Benchmark("archIVD1s", "in_vitro_diagnostics_on_archIVD1s"));
		benchmarks.push_back(Benchmark("archIVD2s", "in_vitro_diagnostics_on_archIVD2s"));
		benchmarks.push_back(Benchmark("archPCR1s", "PCR_on_archPCR1s"));
		benchmarks.push_back(Benchmark("archPCR2s", "PCR_on_archPCR2s"));
		benchmarks.push_back(Benchmark("archPCR3s", "PCR_on_archPCR3s"));
		
		bool benchmarks_failed = false;
		
		for(Benchmark bcm : benchmarks) {
						
			benchmarks_failed = !bcm.run(benchmark_flags);
			if(benchmarks_failed) break;
		}
		
		if(!benchmarks_failed) {
		
			std::cout << "Benchmarks completed successfully";
			
		//	if(benchmarks_time) {
		//		std::cout << " in " <<  (end_time - start_time);
		//	}
				
			std::cout << ".\n";
		}
	}
	
	// dump output to log
//	util::writeToFile("log.txt", output_capture.str());
	
}
