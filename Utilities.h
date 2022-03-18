#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace Utilities
{
	bool isNan(std::string stringa);
	std::vector<char> searchOperatori(std::string stringa);
	std::vector<std::string> OperatoriParole(std::string stringa);
	std::vector<std::string> split(std::string stringa, char splitter);
	bool findInVector(std::vector<std::string> lista, std::string key);
};