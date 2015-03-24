#ifndef TEST_H
#define TEST_H

class Test {
public:
	 
	Test();
	Test(std::string, std::string);
	~Test();
	
	bool run(int flags);
	
private:
	
	std::map<std::string, std::string> getExpectedData();
	bool saveExpectedData();
	
	// TODO: make this user-accessible
	std::vector<std::string> test_data_fields = {
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

#endif // TEST_H
