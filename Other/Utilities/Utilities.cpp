#include "Utilities.h"
#include "../../Interpreter/Interpreter.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace interpreter;
using Utilities::to_string;

vector<string> Utilities::split(string String, char splitter)
{
	vector<string> operators = {
		"+",
		"-",
		"/",
		"*",
		"=",
		"(",
		")",
		",",
		"{",
		"}",
		";",
		"<",
		">",
		"/",
        "&",
        "[",
        "]",
        "."
	};
	string word;
	vector<string> words;
	bool isString = false;
	try
	{
		for (int i = 0; i < (String.length()); i++)
		{
			char character = String[i];
			if (((character == splitter && !isString) || i == String.length() - 1))
			{
				//cout << word << endl;
				if (character == splitter && !word.empty() && !isString) {
					words.push_back(word);
					word = "";
				}
				else if (i == String.length() - 1)
				{
					if (findInVector(operators, string(1, character)))
					{
						if (!word.empty())
							words.push_back(word);
						words.emplace_back(1, character);
						word = "";
					}
					else
					{
						word += character;
						words.push_back(word);
						word = "";
					}
				}
			}
			else
			{
				string char_to_string = string(1, character);
				if (character == '"')
				{
                    isString = !isString;
					word += character;
				}
				else if (findInVector(operators, char_to_string) && !isString)
				{
					if (!word.empty())
						words.push_back(word);
					words.emplace_back(1, character);
					word = "";
				}
				else
				{
					if (character != '\t')
						word += character;
				}
			}
		}
	}
	catch (const exception& test)
	{
		cout << test.what() << endl;
	}
	return words;
}

bool Utilities::findInVector(const vector<string>& list, const string& key)
{
	bool found = false;
	for (auto & i : list)
	{
		if (i == key)
		{
            found = true;
			break;
		}
	}
	return found;
}

bool Utilities::to_bool(const std::string& String)
{
    if (String == "true")
        return true;
    else
        return false;
}

string Utilities::to_string(bool Bool)
{
    if (Bool)
        return "true";
    else
        return "false";
}

bool Utilities::isNan(const string& string)
{
	try
	{
        int integer = stoi(string);
		return false;
	}
	catch (const exception&)
	{
		return true;
	}
}

string Utilities::getTypeVar(std::string val)
{
    if (isNan(val))
    {
        if (val[0] == '"')
            return "string";
        else if (val[0] == '[')
            return "list";
        else if (val == "true" || val == "false")
            return "bool";
        else
            return "";
    }
    else
    {
        //cout << "returning int" << endl;
        return "int";
    }
}

Variable Interpreter::find_variable(const string& name)
{
    //bool found = false;
    Variable VAR;

    for (auto & variable : this->variables)
    {
        Variable* var = &variable;
        if (var->get_name() == name)
        {
            VAR = *var;
            break;
        }
    }
    //cout << this->line << " " << VariablesInfos.size() << endl;
    for (auto & VariablesInfo : this->VariablesInfos)
    {
        for (auto & i2 : VariablesInfo)
        {
            Variable* var = &i2;
            //cout << "size2 = " << VariablesInfos.size() << endl;
            //cout << "name2 = " << var->get_name() << endl;
            if (var->get_name() == name)
            {
                VAR = *var;
                break;
            }
        }
    }
    return VAR;
}

Variable* Interpreter::find_variable_pointer(const string& name)
{
    //bool found = false;
    Variable var;
    Variable* VAR = &var;
    for (auto & variable : this->variables)
    {
        Variable* local_var = &variable;
        if (local_var->get_name() == name)
        {
            VAR = local_var;
            break;
        }
    }
    for (auto & VariablesInfo : this->VariablesInfos)
    {
        for (auto & i2 : VariablesInfo)
        {
            Variable* local_var = &i2;
            if (local_var->get_name() == name)
            {
                VAR = local_var;
                break;
            }
        }
    }
    return VAR;
}

void Interpreter::printString(string String)
{
    for (int i = 0; i < String.length(); i++)
    {
        char character = String[i];
        if (character != '"')
        {
            if (character == '\\')
            {
                string word = (string(1, character) + string(1, String[++i]));
                if (word == "\\n")
                {
                    cout << endl;
                }
            }
            else
            {
                cout << character;
            }
        }
    }
}

string Utilities::GetListValue(Variable variable)
{
    string String = "[";
    vector<Variable> items = variable.get_list_value();
    for (int j = 0; j < items.size(); j++) {
        Variable item = items[j];
        string type = item.get_type();
        if (type == "string")
            String += item.get_str_value();
        else if (type == "int")
            String += std::to_string(item.get_int_value());
        else if (type == "bool")
            String += to_string(item.get_bool_value());
        else if (type == "list")
            String += GetListValue(item);
        if (j != items.size()-1)
            String += ", ";
    }
    String += "]";
    return String;
}

void Interpreter::printList(Variable variable)
{
    string String = "[";
    vector<Variable> items = variable.get_list_value();
    for (int j = 0; j < items.size(); j++) {
        Variable item = items[j];
        string type = item.get_type();
        //cout << "type = " << type << endl;
        if (type == "string")
            String += item.get_str_value();
        else if (type == "int")
            String += to_string(item.get_int_value());
        else if (type == "bool")
            String += to_string(item.get_bool_value());
        else if (type == "list")
            String += Utilities::GetListValue(item);
        if (j != items.size()-1)
            String += ", ";
    }
    String += "]";
    cout << String;
}

void Interpreter::PrintError(const string& param_error)
{
    cout << "Error at line " << this->line << ":" << endl
         << param_error << endl;
    this->error = true;
}