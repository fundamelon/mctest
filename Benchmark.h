#ifndef BENCHMARK_H
#define BENCHMARK_H

class Benchmark {
public:
	 
	Benchmark();
	Benchmark(std::string, std::string);
	~Benchmark();
	
	bool run(int flags);
	
private:
	
	std::map<std::string, std::string> getExpectedData();
	
	// TODO: make this user-accessible
	std::vector<std::string> benchmark_data_fields = {
		"Runtime",
		"Width",
		"Height",
		"Area",
		"Component Area",
		"Effective Area",
		"Estimated Intersections",
		"Estimated Length (Avg)",
		"Estimated Length (Total)",
		"Intersections",
		"Length (Avg)",
		"Length (Total)" };
	
	std::string arch_file, app_file;
	
	std::map<std::string, std::string> data_map;
};

#endif // BENCHMARK_H
