#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace Utilities
{
	bool isNan(std::string stringa);
	std::vector<char> searchOperatori(std::string);
	std::vector<std::string> OperatoriParole(std::string);
	std::vector<std::string> split(std::string, char);
	bool findInVector(std::vector<std::string>, std::string);
};