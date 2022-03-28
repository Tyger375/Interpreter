#pragma once
//#include "Utilities.h"
#include "Variable.h"
#include <iostream>
#include <vector>
#include <string>
#include "Function.h"

class Interpreter
{
private:
	int i;
	std::vector<std::string> typeVariables;
	std::vector<Variable> variables;
	std::vector<Function> functions;
	std::vector<bool> Ifs;
	std::vector<std::vector<Variable>> VariablesInfos;
	bool FindindStaple;
public:
	Interpreter();
	void start(std::string);
	void Line(std::string line);
	void debugVariables();
	void debugFunctions();
	Variable find_variable(std::string);
	Function find_function(std::string);
	//void addizione();
	void loadIntVariable(std::vector<std::string>, std::string);
	void loadStringVariable(std::vector<std::string>, std::string);
	void printString(std::string);
	void AddIntegers(std::vector<std::string>, std::string*, std::string*);
	void AddStrings(std::vector<std::string>, std::string*, std::string*);
	void print(std::vector<std::string>);
	void If(const std::string, const std::string, const std::string);
	void FindGraffa(std::vector<std::string>);
};