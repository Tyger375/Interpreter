#include "../Interpreter/Interpreter.h"
#include "./Utilities/Utilities.h"
#include "../Variable/Variable.h"
#include <iostream>

using namespace std;
using namespace Utilities;

void Interpreter::debugVariables()
{
	for (int i = 0; i < this->variables.size(); i++)
	{
		Variable var = variables[i];
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

void Interpreter::debugFunctions()
{
	for (int i = 0; i < this->functions.size(); i++)
	{
		Function func = this->functions[i];
		cout << func.get_name() << endl;
	}
}