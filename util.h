#ifndef UTIL_H
#define UTIL_H

// TODO: make these user-accessible.
#define ARCH_PATH "Benchmarks/Architecture/Synthetic/"
#define APP_PATH "Benchmarks/Application/HighLevelSynthesis/OperationsScheduled/ops/"


enum BENCHMARK_FLAG : int {
	BENCHMARK_FLAG_VERBOSE 			= 1 << 0,	// verbose output
	BENCHMARK_FLAG_SHOW_STATUS 		= 1 << 1,	// show progress bar
	BENCHMARK_FLAG_ABORT_ON_ERR 	= 1 << 2,	// abort on an MCFlow error
	BENCHMARK_FLAG_RECORD			= 1 << 3,	// parse and save benchmark outputs
	BENCHMARK_FLAG_CLEAR 			= 1 << 4	// clear saved benchmark outputs
};

namespace util {
	
	extern std::string main_dir;
	
	std::string runAndCapture(std::string);
	
	std::string readFromFile(std::string path);
	bool writeToFile(std::string path, std::string data);
	
	std::vector<std::string> regexSearch(std::string data, std::string regex);
	
	bool setProgramDir(std::string);
	
	float getTime();
	
	void init();
	
	bool setBenchmarkExpectedData(std::string, std::map<std::string, std::string>);
}


#endif // UTIL_H
