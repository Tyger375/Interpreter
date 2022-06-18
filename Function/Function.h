#ifndef FUNCTION_FUNCTION_H
#define FUNCTION_FUNCTION_H

#include "../Variable/Variable.h"
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;

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
    void set_return(Variable);
    Variable get_return() { return this->ReturnedValue; }
	void setup(string, vector<string>);
	void add_line(const vector<string>&);
	inline vector<vector<string>> get_lines() { return this->lines; };
	inline string get_name() { return this->name; };
	inline vector<string> get_params() { return this->parameters; }
};

#endif