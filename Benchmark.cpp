/* filename: Benchmark.cpp
 * Author: Igi Chorazewicz
 * Date Created: March 16, 2015
 * Description: benchmark class
 */    
 
//TODO: remove dependency on cout
#include <iostream>
 
#include <string>
#include <vector>
#include <map>
 
#include "util.h"
#include "Benchmark.h"

	 
Benchmark::Benchmark() {}

Benchmark::Benchmark(std::string a, std::string b) : arch_file(a), app_file(b){}

Benchmark::~Benchmark() {}

			
bool Benchmark::run(int flags) {
	
	std::string command = "";
	
	// summon MCFlow
	command.append("cd ");
	command.append(util::main_dir);
	command.append(" ; ./MCFlow ");
	
	//TODO: RELATIVE PATHS
	
	// append architecture file at absolute path
	command.append("--arch=");
	command.append(util::main_dir);
	command.append(ARCH_PATH);
	command.append(arch_file);
	command.append(".json");
	
	command.append(" ");
	
	// append application file at absolute path
	command.append("--app=");
	command.append(util::main_dir);
	command.append(APP_PATH);
	command.append(app_file);
	command.append(".json");
	
	// display progress
	if(flags & BENCHMARK_FLAG_SHOW_STATUS)
		std::cout << "." << std::flush;
		
//	end_time = util::getTime();
	
	if(flags & BENCHMARK_FLAG_VERBOSE)
		std::cout << command << std::endl; // if flagged to, dump console command

	// show name of file currently being crunched
	std::cout << "--- " << arch_file << ": " << std::flush;
	
	// execute on the command line, and capture the entire output
	std::string output = util::runAndCapture(command);
	
	// regex for errors
	auto err_set = util::regexSearch(output, "Error.*\n");
	
	if(!err_set.empty()) {
		std::cout << err_set.size();
		std::cout << " error(s) found.\n";
		if(flags & BENCHMARK_FLAG_ABORT_ON_ERR) std::cout << "Aborting.\n";
		else return false;
		
		std::cout << "\n";
	//	std::cout << "--- COMMAND USED: " << command << "\n\n";
	//	std::cout << "--- PROGRAM OUTPUT: \n" << output << "\n\n";
		std::cout << "--- --- ERRORS FOUND: \n";
		
		for(std::string s : err_set)
			std::cout << "--- --- " << s << "\n";
		
		std::cout << "Program failed. Please see error log.\n";
		return false;
		
	} else {
		
		bool output_match = true;
		std::string mismatch_output = "";
	
		std::map<std::string, std::string> expected_data_map;
		
		// skip this if recording data, may cause errors
		if(!(flags & BENCHMARK_FLAG_RECORD))
			expected_data_map = getExpectedData();
			
		if(!expected_data_map.empty()) {
	
			std::string output_parsed = output;
			
			// parse benchmark output per each fieldname
			for(auto fieldname : benchmark_data_fields) {
				
				// split up the output so the next line is the field and data we need
				output_parsed = output.substr(output.find(fieldname), output.length()-1);
				
				// perform simple regex for data field; if valid, write it into data map
				// TODO: BUG: An empty data field causes the next one down to be read instead
				std::string regex_text = "[\\d].*";
				auto result_set = util::regexSearch(output_parsed, regex_text);
				if(result_set.size() != 0)							
					data_map.insert(std::pair<std::string, std::string>(fieldname, result_set.at(0)));
				
				std::string expected_value = expected_data_map[fieldname];
				std::string obtained_value = data_map[fieldname];
				
				// tediously construct output message
				if(expected_value.compare(obtained_value) != 0) {
					output_match = false;
					mismatch_output.append("--- --- ");
					mismatch_output.append(fieldname);
					mismatch_output.append(": Expected <");
					mismatch_output.append(expected_value);
					mismatch_output.append(">, got <");
					mismatch_output.append(obtained_value);
					mismatch_output.append(">\n");
				}
			}
		
		
			std::cout << "output" << std::flush;
			
			if(flags & BENCHMARK_FLAG_RECORD) {
				if(util::setBenchmarkExpectedData(this->arch_file, this->data_map))
					std::cout << " recorded\n" << std::flush;
				else
					std::cout << "failed to record\n";
			} else if(output_match) {
				std::cout << " correct\n";
			} else {
				std::cout << " failed\n";
				std::cout << "--- --- INCORRECT OUTPUTS:\n";
				std::cout << mismatch_output;
			}
		} else {
			std::cout << "Failed to load expected output. Skipping comparison.\n";
		}
		
	}
	
	std::cout << std::flush;
	
	// dump output
	if(flags & BENCHMARK_FLAG_VERBOSE)
		std::cout << output;
	
	return true;
}
	
	
// Parses benchmark data file into a map
std::map<std::string, std::string> Benchmark::getExpectedData() {
	
	std::string path = "benchmark_data/";
	path.append(arch_file);
	path.append(".txt");
	std::string file_data = util::readFromFile(path);
	std::string file_data_parsed = file_data;
	
	std::map<std::string, std::string> map_from_file;
	
	for(auto fieldname : benchmark_data_fields) {
		
		file_data_parsed = file_data.substr(file_data.find(fieldname), file_data.length()-1);
		
		std::string regex_text = "[\\d].*";
		
		auto result_set = util::regexSearch(file_data_parsed, regex_text);
		
		if(result_set.size() != 0)	{	
			map_from_file.insert(std::pair<std::string, std::string>(fieldname, result_set.at(0)));
		}
	}
	
	/*
	map_from_file["Runtime"]
	map_from_file["Width"]
	map_from_file["Height"]
	map_from_file["Area"]
	map_from_file["Component Area"]
	map_from_file["Effective Area"]
	map_from_file["Estimated Intersections"]
	map_from_file["Estimated Length (Avg)"]
	map_from_file["Estimated Length (Total)"]
	map_from_file["Intersections"]
	map_from_file["Length (Avg)"]
	map_from_file["Length (Total)"]
	*/
	
	return map_from_file;
}
