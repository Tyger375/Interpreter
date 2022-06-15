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
    this->ReturnedValue = move(var);
}

void Function::add_line(vector<string> line)
{
	//cout << line.size() << endl;
	// Ao er contributore
    //for (int i = 0; i < line.size(); ++i) {
        //cout << line[i] << endl;
    //}
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
    {
        this->PrintError("No param given");
        return;
    }


    cout << "Output: ";
    for (int i = 0; i < parameters.size(); i++)
    {
        const string parameter = parameters[i];
        if (parameter[0] == '"')
            this->printString(parameter);
        else if (!isNan(parameter))
        {
            cout << stoi(parameter);
        }
        else if (parameter == "true" || parameter == "false")
        {
            cout << parameter;
        }
        else if (parameter[0] == '[')
        {
            int index = 0;
            //this->writingList.push_back(true);
            //cout << "parameter = " << parameter << endl;
            this->loadList(split(parameter, ' '), false, &index);

            this->printList(ListWriting[0]);

            this->ListWriting.erase(ListWriting.end()-1);

            while (!this->writingList.empty())
            {
                this->writingList.erase(writingList.end()-1);
            }
        }
        else
        {
            Variable var = this->find_variable(parameter);
            const string type = var.get_type();
            if (type == "string")
            {
                this->printString(var.get_str_value());
            }
            else if (type == "int")
            {
                cout << var.get_int_value();
            }
            else if (type == "bool")
            {
                bool Val = var.get_bool_value();
                if (Val == true)
                    cout << "true";
                else
                    cout << "false";
            }
            else if (type == "list")
            {
                this->printList(var);
            }
            else
            {
                this->PrintError("Invalid variable");
            }
        }
        cout << " ";
    }
    cout << endl;
}

string Interpreter::Typeof(vector<string> parameters, bool* Returning)
{
    if (parameters.size() == 0)
    {
        this->PrintError("No param given");
        *Returning = false;
        return "";
    }

    const string parameter = parameters[0];
    string Val;

    if (parameter[0] == '"')
    {
        Val = parameter;
    }
    else if (!isNan(parameter))
    {
        Val = parameter;
    }
    else
    {
        Variable var = this->find_variable(parameter);
        if (var.get_type() == "")
        {
            this->PrintError("Invalid variable");
            *Returning = false;
            return "";
        }
        const string type = var.get_type();
        if (type == "string")
        {
            string VALUE = var.get_str_value();
            const string Type2 = getTypeVar(VALUE);
            if (Type2 == "int")
                VALUE.erase(std::remove(VALUE.begin(), VALUE.end(), '"'), VALUE.end());
            else
                VALUE = '"' + VALUE + '"';
            Val = VALUE;
        }
        else if (type == "int")
        {
            Val = to_string(var.get_int_value());
        }
        else if (type == "bool")
        {
            Val = to_string(var.get_bool_value());
        }
        else
        {
            this->PrintError("Invalid variable");
        }
    }

    string type = getTypeVar(Val);
    //cout << Val << " " << type << endl;
    *Returning = true;
    return type;
}

void Interpreter::SetReturnValue(vector<string> splitted)
{
    if ((splitted.size()-1) != i)
    {
        string ReturningVal = splitted[++i];
        string type;
        type = getTypeVar(ReturningVal);
        Variable var;
        if (type == "")
        {
            var = find_variable(ReturningVal);
            if (var.get_type() == "")
            {
                this->PrintError("Invalid variable returned");
                return;
            }
            else
                this->FUNC->set_return(var);
        }
        else
        {
            if (type == "string")
                var.setup("", ReturningVal);
            else if (type == "int")
                var.setup("", stoi(ReturningVal));
            else
            {
                this->PrintError("Invalid type");
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
    else if (namefunction == "in")
    {
        string Var;
        cin >> Var;
        *Returning = true;
        if (isNan(Var))
            Var = '"' + Var + '"';
        ReturnedVariable.setup("", Var);
        return ReturnedVariable;
    }
    else if (namefunction == "type")
    {
        string type = this->Typeof(parameters, Returning);
        //cout << "debugging type = " << type << endl;
        ReturnedVariable.setup("", type);
        return ReturnedVariable;
    }
    else if (namefunction == "if")
    {
        if (!CheckWriting)
        {
            //cout << "if statement" << endl;
            if (parameters.size() < 3)
            {
                this->PrintError("Something missing in if statement");
                return ReturnedVariable;
            }

            /*
            const string if1 = parameters[0];
            const string comparison = parameters[1];
            const string if2 = parameters[2];
            */

            this->If(parameters);
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
                if (IsNewFunc)
                {
                    Function func;
                    func.setup(namefunction, parameters);
                    this->functions.push_back(func);
                }
                else
                {
                    this->PrintError("Invalid function");
                }
            }
            catch (const exception& error)
            {
                this->PrintError("Invalid function");
            }
            *Returning = false;
            return ReturnedVariable;
        }
        else
        {
            this->executeCustomFunction(&func, parameters);

            Variable returnedVar = func.get_return();

            Variable var;

            func.set_return(var);

            return returnedVar;
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
    if (func->get_params().size() > parameters.size())
    {
        this->PrintError("Params");
        return;
    }
    vector<vector<string>> lines = func->get_lines();
    vector<Variable> NewVariables = this->variables;
    vector<Variable> ParamsToVars;

    for (int j = 0; j < parameters.size(); j++)
    {
        Variable var;
        string val = parameters[j];
        if (isNan(val))
        {
            if (val[0] == '"')
            {
                var.setup(func->get_params()[j], val);
            }
            else if (val[0] == '[')
            {
                //this->writingList.push_back(true);
                int index = j;

                this->loadList(split(val, ' '), false, &index);
                var.setup(func->get_params()[j], ListWriting[0].get_list_value());
                this->ListWriting.erase(ListWriting.end()-1);
                if (!writingList.empty())
                    writingList.erase(writingList.end()-1);

                j = index;
            }
            else
            {
                Variable find = this->find_variable(val);
                const string type = find.get_type();
                if (type != "")
                {
                    if (type == "string")
                        var.setup(func->get_params()[j], find.get_str_value());
                    else if (type == "int")
                        var.setup(func->get_params()[j], find.get_int_value());
                    else if (type == "bool")
                        var.setup(func->get_params()[j], find.get_bool_value());
                    else if (type == "list")
                        var.setup(func->get_params()[j], find.get_list_value());
                }
                else
                {
                    this->PrintError("Invalid variable in function");
                    return;
                }
            }
        }
        else
            var.setup(func->get_params()[j], stoi(val));
        NewVariables.push_back(var);
        ParamsToVars.push_back(var);
    }

    Interpreter interpreter(NewVariables, true, func);

    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> line = lines[i];
        string Strline = "";
        for (int i2 = 0; i2 < line.size(); i2++)
        {
            Strline += (line[i2] + " ");
        }
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