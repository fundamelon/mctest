/* filename: util.cpp
 * Author: Igi Chorazewicz
 * Date Created: December 20, 2014
 * Description: test harness utility functions
 */    
 
 
#include <ctime>
 
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
 
#include "util.h"

namespace util {
	
	std::string main_dir = "";
	
	// Initialize
	void init() {
		
		// read mcflow directory from file
		main_dir = readFromFile("dir.txt");
	}
	
	// Executes a command in terminal, and records its output.
	std::string runAndCapture(std::string cmd) {
		
		// adapted from https://www.jeremymorgan.com/
		std::string data;
		FILE* stream;
		const int max_buffer = 256;
		char buffer[max_buffer];
		cmd.append(" 2>&1");

		stream = popen(cmd.c_str(), "r");
		
		if (stream) {
			while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
			pclose(stream);
		}
		
		return data;
	}
	
	
	// Simple function to read plain text file to string.
	std::string readFromFile(std::string path) {
		
		std::string data = "";
		
		std::ifstream dir_file(path.c_str());
		std::string line;
		
		// TODO: Perhaps more error checking, this is getting scary.
		
		if(dir_file.is_open()) {
			while(getline(dir_file, line))
				data.append(line + "\n");
			dir_file.close();
		}
		
		return data;
	}
	
	
	// Writes string to plain text file.
	bool writeToFile(std::string path, std::string data) {
		
		
		bool success = false;
		std::ofstream dir_file(path.c_str());
		
		if(dir_file.is_open()) {
			dir_file << data;
			dir_file.close();
			success = true;
		}
		
		return success;
	}
	
	
	// Overwrites dir.txt with new directory
	bool setProgramDir(std::string new_dir) {
		
		return writeToFile("dir.txt", new_dir);
	}
	
	
	// Simple regex search function that returns vector of string matches (Relies on C++11, use sparingly)
	std::vector<std::string> regexSearch(std::string data, std::string regex_param) {
		
		std::regex regex_text(regex_param);
		std::sregex_iterator data_iter(data.begin(), data.end(), regex_text);
		std::sregex_iterator data_end;
		
		std::vector<std::string> match_set;
			
		while(data_iter != data_end) { 
			match_set.push_back((*data_iter).str());
			++data_iter;
		}
		
		return match_set;
	}
	
	
	// Writes a map of benchmark data into a corresponding data file.
	bool setBenchmarkExpectedData(std::string benchmark_name, std::map<std::string, std::string> map_from_benchmark) {
		
		std::stringstream file_contents;
		
	//	std::cout << path << "\n";
		std::string path = "benchmark_data/";
		path.append(benchmark_name);
		path.append(".txt");
		
		for(auto p : map_from_benchmark) {
			file_contents << p.first << " " << p.second << "\n";
		}
		
		return writeToFile(path, file_contents.str());
	}
	
	
	// placeholder
	float getTime() { return 0;	}
}