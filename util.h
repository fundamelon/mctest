#ifndef UTIL_H
#define UTIL_H

// TODO: make these user-accessible.
#define ARCH_PATH "Benchmarks/Architecture/Synthetic/"
#define APP_PATH "Benchmarks/Application/HighLevelSynthesis/OperationsScheduled/ops/"

#define SGR_RESET 	"\033[0m"
#define SGR_RED		"\033[1;31m"

enum BENCHMARK_FLAG : int {
	RUN_ALL			= 1 << 0	// Run all default benchmark files
}; 

enum TEST_FLAG : int {
	VERBOSE 		= 1 << 0,	// verbose output
	SHOW_STATUS 	= 1 << 1,	// show progress bar
	ABORT_ON_ERR 	= 1 << 2,	// abort on an MCFlow error
	SAVE			= 1 << 3,	// parse and save test output
	CLEAR 			= 1 << 4,	// clear saved test output
	ANALYZE_OUTPUT	= 1 << 5	// Print output analysis on test conclusion
};

namespace util {
	
	extern std::string main_dir;
	extern std::map<std::string, std::string> settings_map;
	
	std::string runAndCapture(std::string);
	
	bool isValidDir(const std::string&);
	
	std::string readFromFile(const std::string& path);
	bool writeToFile(std::string path, std::string data);
	
	std::vector<std::string> regexSearch(std::string data, std::string regex_param);
	std::string regexSingleSearch(std::string data, std::string regex_param);
	
	bool setProgramDir(const std::string&);
	
	bool createSettingsFile();
	bool parseSettingsFile();
	bool writeSettingsFile();
	
	float getTime();
	
	void init();
	void close();
}


#endif // UTIL_H
