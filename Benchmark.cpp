/* filename: Benchmark.cpp
 * Author: Igi Chorazewicz
 * Date Created: March 16, 2015
 * Description: benchmark class
 */    
 
 
//TODO: remove dependency on cout
#include <iostream>
 
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <cmath>
 
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
	if(flags & BENCHMARK_FLAG::SHOW_STATUS)
		std::cout << "." << std::flush;
		
//	end_time = util::getTime();
	
	if(flags & BENCHMARK_FLAG::VERBOSE)
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
		if(flags & BENCHMARK_FLAG::ABORT_ON_ERR) std::cout << "Aborting.\n";
		else return false;
		
		std::cout << "\n";
		std::cout << "--- --- ERRORS FOUND: \n";
		
		for(std::string s : err_set)
			std::cout << "--- --- " << s << "\n";
		
		std::cout << "Program failed. Please see error log.\n";
		return false;
		
	} else {
		
		bool output_match = true;
		bool output_ignore = false;
		std::stringstream output_comparison;
		std::streamsize default_ss = output_comparison.precision();
	
		std::map<std::string, std::string> expected_data_map;
		
		// skip loading data if saving data, may cause errors
		if(!(flags & BENCHMARK_FLAG::SAVE)) {
		
			expected_data_map = getExpectedData();
			output_ignore = expected_data_map.empty(); // ignore comparison if no data
		}
		
		std::string output_parsed = output;
		
		// parse benchmark output per each fieldname
		for(auto fieldname : benchmark_data_fields) {
		
			// split up the output so the next line is the field and data we need
			output_parsed = output.substr(output.find(fieldname), output.length()-1);
			
			// perform simple regex for data field; if valid, write it into data map
			// TODO: BUG: An empty data field causes the next one to be read instead
			std::string regex_text = "[\\d].*";
			std::string result_text = util::regexSingleSearch(output_parsed, regex_text);
			data_map.insert(std::pair<std::string, std::string>(fieldname, result_text));
			
			if(expected_data_map.empty()) continue; // skip comparison if no data on file
			
			std::string expected_literal = expected_data_map[fieldname];
			std::string obtained_literal = data_map[fieldname];
			
			if(expected_literal.compare(obtained_literal) != 0) { // string comparison only
				
				output_match = false;
				
				// translate literals into data
				double expected_value = stod(expected_literal, 0);
				double obtained_value = stod(obtained_literal, 0);
				
				// construct output comparison
				// TODO: format this nicely!
				output_comparison << "--- --- " << fieldname << ": Expected <";
				output_comparison << expected_value << ">, got <" << obtained_value << ">";
				
				double diff_percent = (obtained_value - expected_value) / expected_value * 100.0;
				
				output_comparison.precision(2);
				output_comparison << " (" << std::fabs(diff_percent) << "% ";
				output_comparison.precision(default_ss);
				
				output_comparison << ((diff_percent >= 0) ? "increase" : "decrease") << ")\n";
			
			}
		}
		
		std::cout << std::flush;
		
		if(flags & BENCHMARK_FLAG::SAVE) {
			if(saveExpectedData())
				std::cout << "Output recorded\n" << std::flush;
			else
				std::cout << "Error: failed to save data\n";
		} else if(output_ignore) {
			std::cout << "Error: no data on file.  Run <./mctest -Bsave> to save output snapshots.\n";
		} else if(output_match) {
			std::cout << "Output match\n";
		} else {
			std::cout << "Output altered\n";
			std::cout << "--- --- OUTPUT ANALYSIS:\n";
			std::cout << output_comparison.str();
		}
		
	}
	
	std::cout << std::flush;
	
	// dump output
	if(flags & BENCHMARK_FLAG::VERBOSE)
		std::cout << output;
	
	return true;
}
	
	
// Parses benchmark data file into a map
std::map<std::string, std::string> Benchmark::getExpectedData() {
	
	std::map<std::string, std::string> map_from_file;
	
	std::string path = "benchmark_data/";
	path.append(arch_file);
	path.append(".txt");
	std::string file_data = util::readFromFile(path);
	
	if(file_data == "") return map_from_file;
	
	std::string file_data_parsed = file_data;
	
	for(auto fieldname : benchmark_data_fields) {
		
		file_data_parsed = file_data.substr(file_data.find(fieldname), file_data.length()-1);
		
		std::string regex_text = "[\\d].*";
		
		auto result_set = util::regexSearch(file_data_parsed, regex_text);
		
		if(result_set.size() != 0)	{	
			map_from_file.insert(std::pair<std::string, std::string>(fieldname, result_set.at(0)));
		}
	}
	
	return map_from_file;
}

	
	
	// Writes a map of benchmark data into a corresponding data file.
bool Benchmark::saveExpectedData() {
	
	std::stringstream file_contents;
	
//	std::cout << path << "\n";
	std::string path = "benchmark_data/";
	
	if(!util::isValidDir(path)) {
		util::runAndCapture("mkdir benchmark_data");
		if(!util::isValidDir(path)) return false;
		else std::cout << "Created directory " << path << ". ";
	}
	
	path.append(arch_file);
	path.append(".txt");
	
	for(auto p : data_map) {
		file_contents << p.first << " " << p.second << "\n";
	}
	
	return util::writeToFile(path, file_contents.str());
}
