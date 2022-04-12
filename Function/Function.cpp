#include "../Other/Utilities/Utilities.h"
#include "Function.h"
#include "../Interpreter/Interpreter.h"
#include <iostream>
#include <vector>
#include <string>
#include "../Variable/Variable.h"

using namespace std;
using namespace Utilities;
using namespace interpreter;

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
            //cout << "Debug param: " << parameter << endl;
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

Variable Interpreter::executeFunction(const string& namefunction, bool CheckWriting, vector<string> parameters, bool IsNewFunc, bool* Returning)
{
    Variable ReturnedVariable;
    if (namefunction == "out" && !CheckWriting)
    {
        this->print(parameters);
        ReturnedVariable.setup("", "");
        *Returning = true;
        return ReturnedVariable;
    }
    else if (namefunction == "if")
    {
        if (!CheckWriting)
        {
            //cout << "if statement" << endl;
            const string if1 = parameters[0];
            const string comparison = parameters[1];
            const string if2 = parameters[2];


            this->If(if1, comparison, if2);
        }
        else
        {
            this->Ifs.push_back(true);
        }
        ReturnedVariable.setup("", "");
        *Returning = true;
        return ReturnedVariable;
    }
    else if (namefunction == "for")
    {
        cout << "for loop" << endl;
        int num = 0;
        vector<string> assign;
        vector<string> check;
        vector<string> advancing;
        for (int index = 0; index < parameters.size(); index++)
        {
            if (parameters[index] != ";")
            {
                if (num == 0)
                {
                    assign.push_back(parameters[index]);
                }
                else if (num == 1)
                {
                    check.push_back(parameters[index]);
                }
                else if (num == 2)
                {
                    advancing.push_back(parameters[index]);
                }
                else
                    break;
            }
            else
            {
                num++;
            }
        }

        this->ForLoop(assign, check, advancing);
        ReturnedVariable.setup("", "");
        *Returning = true;
        return ReturnedVariable;
    }
    else if (namefunction == "while")
    {
        this->WhileLoop(parameters);
        ReturnedVariable.setup("", "");
        *Returning = true;
        return ReturnedVariable;
    }
    else if (!CheckWriting)
    {
        Function func = this->find_function(namefunction);
        if (func.get_name() == "")
        {
            try
            {
                //cout << "isNewFunc = " << IsNewFunc << endl;
                if (IsNewFunc)
                {
                    //cout << "Aggiungo funzione" << endl;
                    Function func;
                    func.setup(namefunction, parameters);
                    //writingFunc = true;
                    this->functions.push_back(func);
                    //cout << functions.size() << endl;
                }
                else
                {
                    cout << "Error: invalid function" << endl;
                }
            }
            catch (const exception& error)
            {
                //cout << error.what() << endl;
                cout << "Error: invalid function" << endl;
            }
            *Returning = false;
            return ReturnedVariable;
        }
        else
        {
            //vector<string> funcparams = func.get_params();
            this->executeCustomFunction(&func, parameters);
            //cout << "returned value type: " << func.get_return().get_type() << endl;

            Variable returnedVar = func.get_return();

            Variable var;

            func.set_return(var);

            //cout << func.get_return().get_value() << " " << returnedVar.get_value() << endl;

            return returnedVar;
            //cout << "Funzione esistente" << endl;
        }
    }
    else
    {
        *Returning = false;
        return ReturnedVariable;
    }
}

void Interpreter::executeCustomFunction(Function* func, vector<string> parameters)
{
    if (func->get_params().size() != parameters.size())
    {
        cout << "Error: params" << endl;
        return;
    }
    vector<vector<string>> lines = func->get_lines();
    vector<Variable> NewVariables = this->variables;
    vector<Variable> ParamsToVars;

    for (int i = 0; i < parameters.size(); i++)
    {
        Variable var;
        string val = parameters[i];
        //cout << "val = " << val << endl;
        if (isNan(val))
        {
            //cout << "adding var" << endl;
            if (val[0] == '"')
            {
                //cout << "adding " << val << endl;
                var.setup(func->get_params()[i], val);
            }
            else
            {
                Variable find = this->find_variable(val);
                const string type = find.get_type();
                if (type != "")
                {
                    if (type == "string")
                    {
                        var.setup(func->get_params()[i], find.get_str_value());
                    }
                    else if (type == "int")
                    {
                        var.setup(func->get_params()[i], find.get_int_value());
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
            var.setup(func->get_params()[i], stoi(val));
        NewVariables.push_back(var);
        ParamsToVars.push_back(var);
    }

    /*cout << "printing vars" << endl;
    for (int j = 0; j < NewVariables.size(); ++j)
    {
        cout << NewVariables[j].get_name() << " " << NewVariables[j].get_value() << endl;
    }*/

    Interpreter interpreter(NewVariables, true, func);

    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> line = lines[i];
        string Strline = "";
        for (int i2 = 0; i2 < line.size(); i2++)
        {
            Strline += (line[i2] + " ");
        }
        //cout << Strline << endl;
        interpreter.Line(Strline);
    }

    vector<Variable> NEWVARIABLES;

    for (int j = 0; j < interpreter.getVariables().size(); ++j) {
        Variable VAR = interpreter.getVariables()[j];
        bool found = false;
        for (int k = 0; k < ParamsToVars.size(); ++k) {
            Variable VARPARAM = ParamsToVars[k];
            if (VAR.get_name() == VARPARAM.get_name()) {
                found = true;
                break;
            }
        }
        if (!found)
            NEWVARIABLES.push_back(VAR);
    }

    //interpreter.debugVariables();
    /*for (int j = 0; j < interpreter.getVariables().size(); ++j)
    {
        cout << interpreter.getVariables()[j].get_name() << " " << interpreter.getVariables()[j].get_value() << endl;
    }*/
    this->variables = NEWVARIABLES;
    return;
}