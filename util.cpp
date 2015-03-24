/* filename: util.cpp
 * Author: Igi Chorazewicz
 * Date Created: December 20, 2014
 * Description: test harness utility functions
 */    
 
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // unix only
 
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>
 
#include "util.h"

namespace util {
	
	std::string main_dir = "";
	std::map<std::string, std::string> settings_map;
	
	// Initialize
	void init() {
		
		// read mcflow directory from file
		main_dir = readFromFile("dir.txt");
		
		// parse settings
		parseSettingsFile();
	}
	
	
	// Save and exit
	void close() {
		
		writeSettingsFile();
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
	
	
	// Check if directory exists
	bool isValidDir(const std::string& path) {
		
		struct stat statbuf;
		
		return (stat(path.c_str(), &statbuf) == 0 && S_ISDIR(statbuf.st_mode));
	}
	
	
	// Simple function to read plain text file to string.
	std::string readFromFile(const std::string& path) {
		
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
	bool setProgramDir(const std::string& new_dir) {
		
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
	
	
	// simple wrapper to take care of returning only first result, with error checking.
	std::string regexSingleSearch(std::string data, std::string regex_param) {
		
		auto match_set = regexSearch(data, regex_param);
		if(match_set.size() == 0) return "";
		return match_set.at(0);
	}
	
	
	// Create settings.dat
	bool createSettingsFile() {
		
		runAndCapture(">> settings.dat");
		
		return true;
	}
	
	
	// Read and parse data from settings.dat
	bool parseSettingsFile() {
		
		std::string settings_text = readFromFile("settings.dat");
		if(settings_text == "") {
			std::cout << "*** settings.dat does not exist, creating...";
			if(createSettingsFile())
				std::cout << " done\n";
			else {
				std::cout << " ERROR: Could not create settings.dat\n";
				return false;
			}
		}
		
		std::string regex_text = "=[a-zA-Z]*";
		
		std::vector<std::string> settings_data = regexSearch(settings_text, regex_text);
		
		for(auto s : settings_data) {
			
		}
		
		return true;
	}
	
	
	bool writeSettingsFile() {
		
		return true;
	}
	
	
	// placeholder
	float getTime() { return 0;	}
}
