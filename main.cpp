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

#include "util.h" // includes <string>, <vector>, <map>
#include "Benchmark.h"
 
 //Flags 
 // TODO: make these actual flags :(
bool abort_enabled = false;

bool automation_enabled = false;

int test_flags = 0;
int benchmark_flags = 0;

bool benchmarks_enabled = false;
bool test_time = false;


// Array and indexing enum for subopts
enum {
	OPT_TEST_VERB = 0,
	OPT_TEST_SAVE,
	OPT_TEST_CLEAR,
	OPT_TEST_ARCH,
	OPT_TEST_APP,
	OPT_TEST_SELF
};

const char *opt_map[] = {
	[OPT_TEST_VERB] = "verb",
	[OPT_TEST_SAVE] = "save",
	[OPT_TEST_CLEAR] = "clear",
	[OPT_TEST_ARCH] = "ARCH",
	[OPT_TEST_APP] = "APP",
	[OPT_TEST_SELF] = "test",
	NULL
};
 
 
int main(int argc,char *argv[]) {
	
	//TODO: All COUT are temporary.  Create custom stream that also
	//	redirects to a log file.
	
	std::cout << "\n-- MCFlow Test Harness --\n\n";
	
	
	// parse command line options
	char *subopts, *value;
	int opt;
	opterr = 0;
	
	bool test_single = false;
	std::string test_arch, test_app;
	
	while((opt = getopt(argc, argv, "AaBbD:d:HhT:t:")) != -1) {
		std::string arg = (optarg != NULL ? optarg : "");
		switch(opt) {
				// set to abort on unsuccessful run
				case 'A':
				case 'a':
					abort_enabled = true;
					break;
					
				// set to run benchmarks
				case 'B':
				case 'b':
					benchmark_flags |= BENCHMARK_FLAG::RUN_ALL;
					break;
					
				case 'T':
				case 't':
					subopts = optarg;
					while(*subopts != '\0') {
						switch(getsubopt(&subopts, (char **)opt_map, &value)) {
							case OPT_TEST_VERB:
								test_flags |= TEST_FLAG::VERBOSE;
								break;
							case OPT_TEST_SAVE:
								test_flags |= TEST_FLAG::SAVE;
								break;
							case OPT_TEST_CLEAR:
								test_flags |= TEST_FLAG::CLEAR;
								break;
							case OPT_TEST_ARCH:
								if(value == NULL) abort();
								test_single = true;
								test_arch = value;
								break;
							case OPT_TEST_APP:
								if(value == NULL) abort();
								test_single = true;
								test_app = value;
								break;
							case OPT_TEST_SELF:
								test_flags |= TEST_FLAG::SELF;
								break;
							default:
						std::cout << "TEST\n";	
								std::cout << "Invalid argument \"" << value << "\".\n";
								return 1;
						}
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
	
	util::init();
	
	if(util::main_dir == "") {
		std::cout << "Load from previous directory...\n";
	} else if(util::main_dir[util::main_dir.length()-1] == '\n') {
		util::main_dir.erase(util::main_dir.length()-1); // strip newline
	}

	std::cout << "*** MCFlow Directory: ";
	std::cout << (util::main_dir == "" ? "(not set)" : util::main_dir) << "\n\n";
	
	// Benchmark setup
	Benchmark bench;
	
	if(test_flags & TEST_FLAG::VERBOSE) std::cout << "*** Tests set to verbose.\n";
	if(test_flags & TEST_FLAG::SAVE) std::cout << "*** Tests will record output.\n";
	
	if(test_arch != "") std::cout << "*** Test arch file: " << test_arch << '\n';
	else if(test_single) std::cout << "*** ERROR: Please specify an arch path with the app path.\n";
	
	if(test_app != "") std::cout << "*** Test app file: " << test_app << '\n';
	else if(test_single) std::cout << "*** ERROR: Please specify an arch path with the app path.\n";
	
	if(test_arch != "" && test_app != "") benchmark_flags |= BENCHMARK_FLAG::RUN_SINGLE;
	
	
	bench.setTestPaths(test_arch, test_app); // ignored if benchmarking all
	bench.setRepeatAmount(1);
		
	bench.run(benchmark_flags, test_flags);
	
	// dump output to log
//	util::writeToFile("log.txt", output_capture.str());
	
}
