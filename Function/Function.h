#pragma once

#include "../Variable/Variable.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Function
{
private:
	string name;
	vector<vector<string>> lines;
	vector<string> parameters;
public:
	void setup(string, vector<string>);
	void add_line(vector<string>);
	inline vector<vector<string>> get_lines() { return this->lines; };
	inline string get_name() { return this->name; };
	inline vector<string> get_params() { return this->parameters; }
};