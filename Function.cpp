#include "Utilities.h"
#include "Function.h"
#include "Interprete.h"
#include <iostream>
#include <vector>
#include <string>
#include "Variable.h"

using namespace std;
using namespace Utilities;

void Function::setup(string name)
{
	this->nome = name;
}

void Function::add_line(vector<string> linea)
{
	this->linee.push_back(linea);
}

Function Interprete::find_function(string name)
{
	bool trovata = false;
	for (int i = 0; i < this->funzioni.size(); i++)
	{
		Function* func = &this->funzioni[i];
		if (func->get_name() == name)
		{
			trovata = true;
			//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
			return *func;
		}
	}
	if (!trovata)
	{
		Function func;
		//cout << var.get_str_value() << endl;
		return func;
	}
}

void Interprete::print(vector<string> parametri)
{
	if (parametri.size() == 0)
		cout << "Error: no param given" << endl;
	else
	{
		const string primoparametro = parametri[0];
		//cout << parametri[0] << " " << parametri[1] << endl;
		if (primoparametro[0] == '"')
		{
			//cout << primoparametro << " e' una stringa" << endl;
			cout << "Output: ";
			this->printString(primoparametro);
		}
		else if (!isNan(primoparametro))
		{
			cout << "Output: " << stoi(primoparametro) << "\n";
		}
		else
		{
			//cout << primoparametro << " e' una variabile" << endl;
			Variable var = this->find_variable(primoparametro);
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