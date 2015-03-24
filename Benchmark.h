#ifndef BENCHMARK_H
#define BENCHMARK_H

class Benchmark {
	
public:
	Benchmark();
	~Benchmark();
	
	// run benchmarks with flag args
	void run(int &, int &);
	
	// set single test paths, ignored if benchmarking all
	void setTestPaths(std::string, std::string);
	
	// set amount to repeat a benchmark, if needed
	void setRepeatAmount(int amt) {repeat_amt = amt;}
	
private:
	std::string single_arch_path;
	std::string single_app_path;
	
	int repeat_amt = 1;
};

#endif // BENCHMARK_H
