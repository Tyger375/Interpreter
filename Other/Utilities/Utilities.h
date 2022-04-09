#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace Utilities
{
	bool isNan(std::string stringa);
	std::vector<std::string> split(std::string, char);
	bool findInVector(std::vector<std::string>, std::string);
    std::string getTypeVar(std::string);
};