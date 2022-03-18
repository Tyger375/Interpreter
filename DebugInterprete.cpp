#include "Interprete.h"
#include "Utilities.h"
#include "Variable.h"
#include <iostream>

using namespace std;
using namespace Utilities;

void Interprete::debugVariables()
{
	for (int i = 0; i < this->variabili.size(); i++)
	{
		Variable var = variabili[i];
		const string type = var.get_type();
		cout << var.get_name();
		if (type == "string")
		{
			cout << " " << var.get_str_value() << endl;
		}
		else if (type == "int")
		{
			cout << " " << var.get_int_value() << endl;
		}
	}
}

void Interprete::debugFunctions()
{
	for (int i = 0; i < this->funzioni.size(); i++)
	{
		Function func = this->funzioni[i];
		cout << func.get_name() << endl;
	}
}