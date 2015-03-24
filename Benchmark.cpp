/* filename: Benchmark.cpp
 * Author: Igi Chorazewicz
 * Date Created: March 16, 2015
 * Description: benchmark class
 */    
 
#include <iostream>
 
#include "util.h" // includes <string>, <vector>, <map>
#include "Test.h"
#include "Benchmark.h"
 
Benchmark::Benchmark() {}
Benchmark::~Benchmark() {}

//Run benchmarks
void Benchmark::run(int &benchmark_flags, int &test_flags) {
	
	if(test_flags & TEST_FLAG::SELF) {
		// Manually set the test parameters and force single.
		setTestPaths("arch10-1s", "synthetic1_on_arch10-1s");
		setRepeatAmount(5);
		benchmark_flags |= BENCHMARK_FLAG::RUN_SINGLE;
	}
	
	for(int bench_iter = 0; bench_iter < repeat_amt; bench_iter++) {
	
		if(benchmark_flags & BENCHMARK_FLAG::RUN_SINGLE) {
			
			std::cout << "[" << bench_iter << "] Running test...\n";
			Test t = Test(single_arch_path, single_app_path);
			bool test_failed = !t.run(test_flags);
			if(test_failed) break;
			
		} else if(benchmark_flags & BENCHMARK_FLAG::RUN_ALL) {
		
			if(util::main_dir == "") {
				std::cout << "Error: Cannot run tests, directory not set!\n";
				std::cout << "Use <./mctest -d \"../path/to/MCFlow/\"> to set.\n";
			} else {
		
				std::cout << "Running benchmarks...\n";
				
				std::vector<Test> tests;
				
			//	float start_time = util::getTime(), end_time;
				
				// TODO: dynamic allocation
				tests.push_back(Test("arch10-1s", "synthetic1_on_arch10-1s"));
				tests.push_back(Test("arch10-2s", "synthetic1_on_arch10-2s"));
				tests.push_back(Test("arch20-1s", "synthetic2_on_arch20-1s"));
				tests.push_back(Test("arch20-2s", "synthetic2_on_arch20-2s"));
				tests.push_back(Test("arch30-1s", "synthetic3_on_arch30-1s"));
				tests.push_back(Test("arch30-2s", "synthetic3_on_arch30-2s"));
				tests.push_back(Test("arch40-1s", "synthetic4_on_arch40-1s"));
				tests.push_back(Test("arch40-2s", "synthetic4_on_arch40-2s"));
				tests.push_back(Test("arch50-1s", "synthetic5_on_arch50-1s"));
				tests.push_back(Test("arch50-2s", "synthetic5_on_arch50-2s"));
				tests.push_back(Test("archIVD1s", "in_vitro_diagnostics_on_archIVD1s"));
				tests.push_back(Test("archIVD2s", "in_vitro_diagnostics_on_archIVD2s"));
				tests.push_back(Test("archPCR1s", "PCR_on_archPCR1s"));
				tests.push_back(Test("archPCR2s", "PCR_on_archPCR2s"));
				tests.push_back(Test("archPCR3s", "PCR_on_archPCR3s"));
				
				bool tests_failed = false;
				
				for(auto t : tests) {
								
					tests_failed = !t.run(test_flags);
					if(tests_failed) break;
				}
				
				if(!tests_failed) {
				
					std::cout << "Benchmark completed successfully";
					
				//	if(benchmarks_time) {
				//		std::cout << " in " <<  (end_time - start_time);
				//	}
						
					std::cout << ".\n";
				}
			}
		}
	}
}


void Benchmark::setTestPaths(std::string arch_path, std::string app_path) {
	single_arch_path = arch_path;
	single_app_path = app_path;
}
