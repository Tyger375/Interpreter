#include "../Other/Utilities/Utilities.h"
#include "Function.h"
#include "../Interpreter/Interpreter.h"
#include <iostream>
#include <vector>
#include <string>
#include "../Variable/Variable.h"

using namespace std;
using namespace Utilities;

void Function::setup(string name)
{
	this->name = name;
}

void Function::add_line(vector<string> line)
{
	//cout << line.size() << endl;
	// Ao er contributore
	this->lines.push_back(line);
}

Function Interpreter::find_function(string name)
{
	//bool found = false;
	Function FUNC;
	for (int i = 0; i < this->functions.size(); i++)
	{
		Function *func = &this->functions[i];
		if (func->get_name() == name)
		{
			//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
			FUNC = *func;
			break;
			//return *func;
		}
	}
	return FUNC;
}

void Interpreter::print(vector<string> parameters)
{
	if (parameters.size() == 0)
		cout << "Error: no param given" << endl;
	else
	{
		cout << "Output: ";
		for (int i = 0; i < parameters.size(); i++)
		{
			const string parameter = parameters[i];
			if (parameter[0] == '"')
			{
				//cout << primoparametro << " e' una stringa" << endl;
				this->printString(parameter);
				//cout << endl;
			}
			else if (!isNan(parameter))
			{
				cout << stoi(parameter);
			}
			else
			{
				//cout << primoparametro << " e' una variabile" << endl;
				Variable var = this->find_variable(parameter);
				const string type = var.get_type();
				if (type == "string")
				{
					this->printString(var.get_str_value());
					//cout << "Output: " << var.get_str_value() << endl;
				}
				else if (type == "int")
				{
					cout << var.get_int_value();
				}
				else
				{
					cout << "Error: invalid variable";
				}
			}
			cout << " ";
		}
		cout << endl;
		//cout << parametri[0] << " " << parametri[1] << endl;
	}
}
