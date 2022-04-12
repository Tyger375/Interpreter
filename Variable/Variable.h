#ifndef VARIABLE_VARIABLE_H
#define VARIABLE_VARIABLE_H
#include <iostream>
#include <vector>
#include <string>

//using namespace std;
//using namespace Utilities;

class Variable;

class Variable
{
public:
	//Variable(std::string nome, int val);
	//Variable(std::string nome, std::string val);
	void setup(std::string nome, int val);
	void setup(std::string nome, std::string val);
	void set_value(int newval);
	void set_value(std::string newval);
	//int get_value();
	//string get_value();
	int get_int_value();
	std::string get_str_value();
	inline std::string get_type() { return this->valueType; }
	inline std::string get_name() { return this->name; }
    std::string get_value();
private:
	int IntValue;
	std::string StrValue;
	std::string valueType;
	std::string name;
};

#endif