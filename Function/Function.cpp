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
    {
        this->PrintError("No param given");
        return;
    }


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
                //cout << "debug = " << var.get_str_value() << endl;
                this->printString(var.get_str_value());
                //cout << "Output: " << var.get_str_value() << endl;
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
            else
            {
                this->PrintError("Invalid variable");
            }
        }
        cout << " ";
    }
    cout << endl;
    //cout << parametri[0] << " " << parametri[1] << endl;
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
        //cout << "returning " << ReturningVal << endl;
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
                    this->PrintError("Invalid function");
                }
            }
            catch (const exception& error)
            {
                //cout << error.what() << endl;
                this->PrintError("Invalid function");
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
        this->PrintError("Params");
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
                    this->PrintError("Invalid variable in function");
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