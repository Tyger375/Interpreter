#include "../Interpreter/Interpreter.h"
#include "./Utilities/Utilities.h"
#include "../Variable/Variable.h"
#include <iostream>

using namespace std;
using namespace Utilities;
using namespace interpreter;

void Interpreter::debugVariables()
{
	for (int i = 0; i < this->variables.size(); i++)
	{
		Variable var = variables[i];
		cout << var.get_name() << " " << var.get_value() << endl;
	}
    cout << "Others: " << endl;
    for (int i = 0; i < this->VariablesInfos.size(); i++)
    {
        vector<Variable> vars = VariablesInfos[i];
        for (int j = 0; j < vars.size(); ++j)
        {
            Variable var = VariablesInfos[i][j];
            cout << var.get_name() << " " << var.get_value() << endl;
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