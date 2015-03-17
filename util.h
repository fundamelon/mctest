#ifndef UTIL_H
#define UTIL_H

// TODO: make these user-accessible.
#define ARCH_PATH "Benchmarks/Architecture/Synthetic/"
#define APP_PATH "Benchmarks/Application/HighLevelSynthesis/OperationsScheduled/ops/"

#define SGR_RESET 	"\033[0m"
#define SGR_RED		"\033[1;31m"


enum BENCHMARK_FLAG : int {
	VERBOSE 		= 1 << 0,	// verbose output
	SHOW_STATUS 	= 1 << 1,	// show progress bar
	ABORT_ON_ERR 	= 1 << 2,	// abort on an MCFlow error
	SAVE			= 1 << 3,	// parse and save benchmark outputs
	CLEAR 			= 1 << 4,	// clear saved benchmark outputs
	RUN_ALL			= 1 << 5,	// Run all default benchmark files
	ANALYZE_OUTPUT	= 1 << 6	// Print output analysis on benchmark conclusion
};

namespace util {
	
	extern std::string main_dir;
	
	std::string runAndCapture(std::string);
	
	bool isValidDir(std::string);
	
	std::string readFromFile(std::string path);
	bool writeToFile(std::string path, std::string data);
	
	std::vector<std::string> regexSearch(std::string data, std::string regex_param);
	std::string regexSingleSearch(std::string data, std::string regex_param);
	
	bool setProgramDir(std::string);
	
	float getTime();
	
	void init();
	
	bool setBenchmarkExpectedData(std::string, std::map<std::string, std::string>);
}


#endif // UTIL_H
