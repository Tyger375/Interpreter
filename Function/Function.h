#ifndef FUNCTION_FUNCTION_H
#define FUNCTION_FUNCTION_H

#include "../Variable/Variable.h"
#include <iostream>
#include <vector>
#include <string>

//using namespace std;

class Function;

class Function
{
private:
    std::string name;
	std::vector<std::vector<std::string>> lines;
	std::vector<std::string> parameters;
    Variable ReturnedValue;
    bool returned;
public:
    bool is_return() { return this->returned; };
    void set_return(Variable);
    Variable get_return() { return this->ReturnedValue; }
	void setup(std::string, std::vector<std::string>);
	void add_line(std::vector<std::string>);
	inline std::vector<std::vector<std::string>> get_lines() { return this->lines; };
	inline std::string get_name() { return this->name; };
	inline std::vector<std::string> get_params() { return this->parameters; }
};

#endif