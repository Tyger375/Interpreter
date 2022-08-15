#ifndef VARIABLE_VARIABLE_H
#define VARIABLE_VARIABLE_H
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <map>

using std::string;
using std::vector;
using std::map;

class Variable;

class Variable
{
public:
	void setup(string var_name, int val);
	void setup(string var_name, string val);
    void setup(string var_name, bool val);
    void setup(string var_name, vector<Variable> val);
    void setup(string var_name, map<string, Variable> val);
    void set_name(string param_name) { this->name = std::move(param_name); }
    //Int value
	inline int get_int_value() const { return this->IntValue; }
    //String value
    inline string get_str_value() { return this->StrValue; }
    inline string* get_str_value_pointer() { return &this->StrValue; }
    //Bool value
    inline bool get_bool_value() const { return this->BoolValue; }
    //List value
    inline vector<Variable> get_list_value() { return this->ListValue; }
    inline vector<Variable>* get_list_value_pointer() { return &this->ListValue; }
    inline void add_item_list(const Variable& var) { this->ListValue.push_back(var); }
    inline Variable* get_item_list_pointer(int index) { return &this->ListValue[index]; }
    //Dict value
    inline map<string, Variable> get_dict_value() { return this->DictValue; }
    inline map<string, Variable>* get_dict_value_pointer() { return &this->DictValue; }
    inline void add_item_dict(const string key, const Variable& value) { this->DictValue.insert({key, value}); }
    inline Variable* get_item_dict_pointer(string key) { return &this->DictValue[key]; }

	inline string get_type() { return this->valueType; }
	inline string get_name() { return this->name; }
    string get_value();
private:
	int IntValue;
    bool BoolValue;
	string StrValue;
    vector<Variable> ListValue;
    map<string, Variable> DictValue;
	string valueType;
	string name;
};

#endif