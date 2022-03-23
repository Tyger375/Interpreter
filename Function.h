#pragma once

#include "Variable.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Function
{
private:
	string name;
	vector<vector<string>> lines;
public:
	void setup(string);
	void add_line(vector<string>);
	inline vector<vector<string>> get_lines() { return this->lines; };
	inline string get_name() { return this->name; };
};