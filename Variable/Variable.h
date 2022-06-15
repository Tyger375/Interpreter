#ifndef VARIABLE_VARIABLE_H
#define VARIABLE_VARIABLE_H
#include <iostream>
#include <vector>
#include <string>
#include <map>

//using namespace std;
//using namespace Utilities;

class Variable;

class Variable
{
public:
	void setup(std::string name, int val);
	void setup(std::string name, std::string val);
    void setup(std::string name, bool val);
    void setup(std::string name, std::vector<Variable> val);
	void set_value(int newval);
	void set_value(std::string newval);
    void set_value(bool newval);
	inline int get_int_value() { return this->IntValue; }
    inline std::string get_str_value() { return this->StrValue; }
    inline bool get_bool_value() { return this->BoolValue; }
    inline std::vector<Variable> get_list_value() { return this->ListValue; }
    inline std::vector<Variable>* get_list_value_pointer() { return &this->ListValue; }
	inline std::string get_type() { return this->valueType; }
	inline std::string get_name() { return this->name; }
    inline void add_item_list(Variable var) { this->ListValue.push_back(var); }
    inline void modify_item_list(Variable var, int index) { this->ListValue[index] = var; }
    inline Variable get_item_list(int index) { return this->ListValue[index]; }
    inline Variable* get_item_list_pointer(int index) { return &this->ListValue[index]; }
    std::string get_value();
private:
	int IntValue;
    bool BoolValue;
	std::string StrValue;
    std::vector<Variable> ListValue;
	std::string valueType = "";
	std::string name;
};

#endif