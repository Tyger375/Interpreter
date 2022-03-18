#pragma once
//#include "Utilities.h"
#include "Variable.h"
#include <iostream>
#include <vector>
#include <string>

class Interprete
{
private:
	int i;
	std::vector<std::string> tipiVariabili;
	std::vector<Variable> variabili;
public:
	Interprete();
	void Linea(std::string linea);
	void debugVariables();
	Variable find_variable(std::string nome);
	//void addizione();
	void loadIntVariable(std::vector<std::string> splitted, std::string name);
	void loadStringVariable(std::vector<std::string> splitted, std::string name);
	void printString(std::string stringa);
	void AddInteri(std::vector<std::string>, std::string*, std::string*);
	void AddStringhe(std::vector<std::string>, std::string*, std::string*);
};