#ifndef VARIABLE_VARIABLE_H
#define VARIABLE_VARIABLE_H
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <map>

using std::string;
using std::vector;

class Variable;

class Variable
{
public:
	void setup(string var_name, int val);
	void setup(string var_name, string val);
    void setup(string var_name, bool val);
    void setup(string var_name, vector<Variable> val);
    void set_name(string param_name) { this->name = std::move(param_name); }
	inline int get_int_value() const { return this->IntValue; }
    inline string get_str_value() { return this->StrValue; }
    inline string* get_str_value_pointer() { return &this->StrValue; }
    inline bool get_bool_value() const { return this->BoolValue; }
    inline vector<Variable> get_list_value() { return this->ListValue; }
    inline vector<Variable>* get_list_value_pointer() { return &this->ListValue; }
	inline string get_type() { return this->valueType; }
	inline string get_name() { return this->name; }
    inline void add_item_list(const Variable& var) { this->ListValue.push_back(var); }
    inline Variable* get_item_list_pointer(int index) { return &this->ListValue[index]; }
    //inline char* get_item_str_pointer(int index) { return &this->StrValue[index]; }
    string get_value();
private:
	int IntValue;
    bool BoolValue;
	string StrValue;
    vector<Variable> ListValue;
	string valueType;
	string name;
};

#endif