#include "../Other/Utilities/Utilities.h"
#include "Function.h"
#include "../Interpreter/Interpreter.h"
#include <iostream>
#include <vector>
#include <string>
#include "../Variable/Variable.h"

using namespace std;
using namespace Utilities;

void Function::setup(string name, vector<string> parameters)
{
	this->name = name;
	this->parameters = parameters;
}

void Function::set_return(Variable var)
{
    this->returned = true;
    this->ReturnedValue = var;
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
            cout << "Debug param: " << parameter << endl;
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

void Interpreter::SetReturnValue(vector<string> splitted)
{
    if ((splitted.size()-1) != i)
    {
        string ReturningVal = splitted[++i];
        //cout << "returning " << ReturningVal << endl;
        string type;
        type = getTypeVar(ReturningVal);
        Variable var;
        if (type == "")
        {
            var = find_variable(ReturningVal);
            if (var.get_type() == "")
            {
                cout << "Error: Invalid variable returned" << endl;
                return;
            }
            else
            {
                this->FUNC->set_return(var);
            }
        }
        else
        {
            if (type == "string")
                var.setup("", ReturningVal);
            else if (type == "int")
                var.setup("", stoi(ReturningVal));
            else
            {
                cout << "Error: Invalid type" << endl;
            }

            this->FUNC->set_return(var);
        }
    }
    else
        cout << "returning nothing" << endl;
}

void Function::execute(vector<string> params, Interpreter* interpreter)
{
    if (this->parameters.size() != params.size())
    {
        cout << "Error: params" << endl;
        return;
    }
    vector<vector<string>> lines = this->get_lines();
    vector<Variable> NewVariables = interpreter->variables;
    for (int i = 0; i < params.size(); i++)
    {
        Variable var;
        string val = parameters[i];
        if (isNan(val))
        {
            if (val[0] == '"')
                var.setup(params[i], val);
            else
            {
                Variable find = interpreter->find_variable(val);
                const string type = find.get_type();
                if (type != "")
                {
                    if (type == "string")
                    {
                        var.setup(params[i], find.get_str_value());
                    }
                    else if (type == "int")
                    {
                        var.setup(params[i], find.get_int_value());
                    }
                }
                else
                {
                    cout << "Error: Invalid variable in function" << endl;
                    return;
                }
            }
        }
        else
            var.setup(params[i], stoi(val));
        NewVariables.push_back(var);
    }

    Interpreter interpreter1(NewVariables, true, this);
    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> line = lines[i];
        string Strline = "";
        for (int i2 = 0; i2 < line.size(); i2++)
        {
            Strline += (line[i2] + " ");
        }
        //cout << Strline << endl;
        interpreter1.Line(Strline);
    }
    //interpreter.debugVariables();
    interpreter->variables = interpreter1.getVariables();
}