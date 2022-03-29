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
	this->lines.push_back(line);
}

Function Interpreter::find_function(string name)
{
	//bool found = false;
	Function FUNC;
	for (int i = 0; i < this->functions.size(); i++)
	{
		Function* func = &this->functions[i];
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
		const string firstparameter = parameters[0];
		//cout << parametri[0] << " " << parametri[1] << endl;
		if (firstparameter[0] == '"')
		{
			//cout << primoparametro << " e' una stringa" << endl;
			cout << "Output: ";
			this->printString(firstparameter);
		}
		else if (!isNan(firstparameter))
		{
			cout << "Output: " << stoi(firstparameter) << "\n";
		}
		else
		{
			//cout << primoparametro << " e' una variabile" << endl;
			Variable var = this->find_variable(firstparameter);
			const string type = var.get_type();
			if (type == "string")
			{
				cout << "Output: ";
				this->printString(var.get_str_value());
				cout << endl;
				//cout << "Output: " << var.get_str_value() << endl;
			}
			else if (type == "int")
			{
				cout << "Output: " << var.get_int_value() << endl;
			}
			else
			{
				cout << "Error: invalid variable" << endl;
			}
		}
	}
}