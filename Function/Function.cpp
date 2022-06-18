#include "../Other/Utilities/Utilities.h"
#include "Function.h"
#include "../Interpreter/Interpreter.h"
#include <iostream>
#include <utility>
#include <vector>
#include <string>

using namespace std;
using namespace Utilities;
using namespace interpreter;

void Function::setup(string name_var, vector<string> parameters_var)
{
	this->name = move(name_var);
	this->parameters = move(parameters_var);
}

void Function::set_return(Variable var)
{
    this->returned = true;
    this->ReturnedValue = move(var);
}

void Function::add_line(const vector<string>& line)
{
	this->lines.push_back(line);
}

Function Interpreter::find_function(const string& name)
{
	//bool found = false;
	Function local_FUNC;
	for (auto & function : this->functions)
	{
		Function *func = &function;
		if (func->get_name() == name)
		{
            local_FUNC = *func;
			break;
			//return *func;
		}
	}
	return local_FUNC;
}

void Interpreter::print(const vector<string>& parameters)
{
	if (parameters.empty())
    {
        this->PrintError("No param given");
        return;
    }


    cout << "Output: ";
    for (auto parameter : parameters)
    {
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
                if (Val)
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
    if (parameters.empty())
    {
        this->PrintError("No param given");
        *Returning = false;
        return "";
    }

    const string parameter = parameters[0];
    string Val;

    if (parameter[0] == '"' || !isNan(parameter))
        Val = parameter;
    else
    {
        Variable var = this->find_variable(parameter);
        if (var.get_type().empty())
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
            {
                VALUE.erase(VALUE.begin());
                VALUE.erase(VALUE.end() - 1);
            }
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
        if (type.empty())
        {
            var = find_variable(ReturningVal);
            if (var.get_type().empty())
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

Variable Interpreter::executeFunction(const string& name_function, bool CheckWriting, const vector<string>& parameters, bool IsNewFunc, bool* Returning)
{
    Variable ReturnedVariable;
    if (name_function == "out" && !CheckWriting)
    {
        this->print(parameters);
        ReturnedVariable.setup("", "");
        *Returning = true;
        return ReturnedVariable;
    }
    else if (name_function == "in")
    {
        string Var;
        cin >> Var;
        *Returning = true;
        if (isNan(Var))
            Var = '"' + Var + '"';
        ReturnedVariable.setup("", Var);
        return ReturnedVariable;
    }
    else if (name_function == "type")
    {
        string type = this->Typeof(parameters, Returning);
        //cout << "debugging type = " << type << endl;
        ReturnedVariable.setup("", type);
        return ReturnedVariable;
    }
    else if (name_function == "if")
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
    else if (name_function == "for")
    {
        cout << "for loop" << endl;
        int num = 0;
        vector<string> assign;
        vector<string> check;
        vector<string> advancing;
        for (auto & parameter : parameters)
        {
            if (parameter != ";")
            {
                if (num == 0)
                {
                    assign.push_back(parameter);
                }
                else if (num == 1)
                {
                    check.push_back(parameter);
                }
                else if (num == 2)
                {
                    advancing.push_back(parameter);
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
    else if (name_function == "while")
    {
        this->WhileLoop(parameters);
        this->FindingFromLine = this->line;
        ReturnedVariable.setup("", "");
        *Returning = true;
        return ReturnedVariable;
    }
    else if (!CheckWriting)
    {
        Function func = this->find_function(name_function);
        if (func.get_name().empty())
        {
            try
            {
                if (IsNewFunc)
                {
                    Function local_func;
                    local_func.setup(name_function, parameters);
                    this->functions.push_back(local_func);
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
            *Returning = true;

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
                if (!type.empty())
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

    //cout << "debugging" << endl;
    //this->debugVariables();

    Interpreter interpreter(NewVariables, true, func);

    /*for (auto LINE : lines)
    {
        for (auto word : LINE)
        {
            cout << word << " ";
        }
        cout << endl;
    }*/

    for (const auto& local_line : lines)
    {
        //cout << local_line[0] << endl;
        string Str_line;
        for (auto & i2 : local_line)
        {
            Str_line += (i2 + " ");
        }
        interpreter.Line(Str_line);
        interpreter.line++;
    }

    vector<Variable> new_variables;

    for (int j = 0; j < interpreter.getVariables().size(); ++j) {
        Variable VAR = interpreter.getVariables()[j];
        bool found = false;
        for (auto var_param : ParamsToVars) {
            if (VAR.get_name() == var_param.get_name()) {
                found = true;
                break;
            }
        }
        if (!found)
            new_variables.push_back(VAR);
    }

    //interpreter.debugVariables();
    /*for (int j = 0; j < interpreter.getVariables().size(); ++j)
    {
        cout << interpreter.getVariables()[j].get_name() << " " << interpreter.getVariables()[j].get_value() << endl;
    }*/
    this->variables = new_variables;
}