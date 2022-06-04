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
	vector<string> operatori = {
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
        "]"
	};
	string word = "";
	vector<string> words;
	bool isStringa = false;
	try
	{
		for (int i = 0; i < (String.length()); i++)
		{
			char pog = String[i];
			//cout << pog << endl;
			//cout << i << stringa.length() << endl;
			if (((pog == splitter && !isStringa) || i == String.length() - 1))
			{
				//cout << word << endl;
				if (pog == splitter && word != "" && !isStringa) {
					words.push_back(word);
					word = "";
				}
				else if (i == String.length() - 1)
				{
					if (findInVector(operatori, string(1, pog)))
					{
						if (word != "")
							words.push_back(word);
						words.push_back(string(1, pog));
						word = "";
					}
					else
					{
						word += pog;
						words.push_back(word);
						word = "";
					}
				}
			}
			else
			{
				string chartostring = string(1, pog);
				if (pog == '"')
				{
					isStringa = !isStringa;
					word += pog;
				}
				else if (findInVector(operatori, chartostring) && !isStringa)
				{
					//cout << word << " " << pog << endl;
					if (word != "")
						words.push_back(word);
					words.push_back(string(1, pog));
					word = "";
				}
				else
				{
					//cout << pog << endl;
					if (pog != '\t')
						word += pog;
				}
			}
			//cout << word << endl;
		}
	}
	catch (const exception& test)
	{
		cout << test.what() << endl;
	}
	/*for (int i = 0; i < (words.size() - 1); i++)
	{
		cout << &words[i] << endl;
	}*/
	return words;
}

bool Utilities::findInVector(vector<string> list, string key)
{
	bool trovato = false;
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == key)
		{
			trovato = true;
			break;
		}
	}
	return trovato;
}

bool Utilities::to_bool(std::string String)
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

bool Utilities::isNan(string String)
{
	try
	{
		int intero = stoi(String);
		return false;
	}
	catch (const exception&)
	{
		return true;
	}
}

string Utilities::getTypeVar(std::string val)
{
    if (isNan(val)) {
        if (val[0] == '"')
            return "string";
        else if (val[0] == '[')
            return "list";
        else if (val == "true" || val == "false")
            return "bool";
        else
            return "";
    } else {
        //cout << "returning int" << endl;
        return "int";
    }
}

Variable Interpreter::find_variable(string name)
{
    //bool found = false;
    Variable VAR;
    for (int i = 0; i < this->variables.size(); i++)
    {
        Variable* var = &this->variables[i];
        if (var->get_name() == name)
        {
            VAR = *var;
            break;
        }
    }
    for (int i = 0; i < this->VariablesInfos.size(); i++)
    {
        for (int i2 = 0; i2 < this->VariablesInfos[i].size(); i2++)
        {
            Variable* var = &this->VariablesInfos[i][i2];
            if (var->get_name() == name)
            {
                VAR = *var;
                break;
            }
        }
    }
    return VAR;
}

Variable* Interpreter::find_variable_pointer(string name)
{
    //bool found = false;
    Variable var;
    Variable* VAR = &var;
    for (int i = 0; i < this->variables.size(); i++)
    {
        Variable* var = &this->variables[i];
        if (var->get_name() == name)
        {
            VAR = var;
            break;
        }
    }
    for (int i = 0; i < this->VariablesInfos.size(); i++)
    {
        for (int i2 = 0; i2 < this->VariablesInfos[i].size(); i2++)
        {
            Variable* var = &this->VariablesInfos[i][i2];
            if (var->get_name() == name)
            {
                static Variable* VAR = var;
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
                string parola = (string(1, character) + string(1, String[++i]));
                if (parola == "\\n")
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

void Interpreter::PrintError(std::string error)
{
    cout << "Error at line " << this->line << ":" << endl
    << error << endl;
    this->error = true;
}