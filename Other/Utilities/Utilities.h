#ifndef UTILITIES_UTILITIES_H
#define UTILITIES_UTILITIES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../../Variable/Variable.h"

using std::vector;
using std::string;

namespace Utilities
{
	bool isNan(const string&);
	vector<string> split(string, char);
	bool findInVector(const vector<string>&, const string&);
    string getTypeVar(string);
    string GetListValue(Variable);
    bool to_bool(const string&);
    string to_string(bool);

};

#endif