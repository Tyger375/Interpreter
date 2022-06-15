#include "../Other/Utilities/Utilities.h"
#include "../Interpreter/Interpreter.h"
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include "Variable.h"

using namespace std;
using namespace Utilities;
using namespace interpreter;

void Interpreter::internal_add(Variable* variable, vector<string> parameters, bool* error)
{
    /*for (int i = 0; i < parameters.size(); ++i) {
        cout << parameters[i] << endl;
    }*/
    if (parameters.empty())
    {
        *error = true;
        return;
    }

    Variable var;
    if (parameters[0][0] == '[')
    {
        int index = 0;
        //this->writingList.push_back(true);
        this->loadList(split(parameters[0], ' '), false, &index);

        //cout << ListWriting.size() << endl;
        var = ListWriting[0];
        this->ListWriting.erase(ListWriting.end()-1);
    }
    else
        var = this->loadVariableWithoutWriting(parameters, "");

    variable->add_item_list(var);
    //variable->ListValue.push_back(var);
}

void Interpreter::execute_internal_function(Variable* variable, const string& functionname, vector<string> parameters, bool* error)
{
    if (variable->get_type() == "list")
    {
        if (functionname == "add")
        {
            this->internal_add(variable, move(parameters), error);
        }
    }
    else
    {
        cout << "here2" << endl;
        *error = true;
    }
}

void Variable::setup(string name, int val)
{
	this->IntValue = val;
	this->name = move(name);
	this->valueType = "int";
}

void Variable::setup(string name, bool val)
{
    this->BoolValue = val;
    this->name = move(name);
    this->valueType = "bool";
}

void Variable::setup(string name, vector<Variable> val)
{
    this->name = move(name);
    this->valueType = "list";
    this->ListValue = move(val);
}

void Variable::setup(string name, string val)
{
    /*if (val[0] != '"' && !val.empty())
    {
        //cout << "adding " << '"' << " to " << val << endl;
        //val = '"' + val + '"';
    }*/
	this->StrValue = move(val);
	this->name = move(name);
	this->valueType = "string";
}

void Variable::set_value(int newval)
{
	this->IntValue = newval;
}

void Variable::set_value(string newval)
{
	this->StrValue = move(newval);
}

void Variable::set_value(bool newval)
{
    this->BoolValue = newval;
}

string Variable::get_value()
{
    if (this->valueType == "string")
        return ('"' + this->StrValue + '"');
    else if (this->valueType == "int")
        return to_string(this->IntValue);
    else if (this->valueType == "bool")
        return to_string(this->BoolValue);
    else if (this->valueType == "list")
        return GetListValue(*this);
    else
        return "";
}

void Interpreter::loadVariable(vector<string> splitted, const string& name)
{
    //cout << this->i << endl;
    string strvalue = splitted[(++this->i)];
    Variable Var = this->find_variable(strvalue);

    string type = Var.get_type();

    if (splitted.size() > (this->i+1) && splitted[this->i + 1] == "(")
    {
        //cout << "function" << endl;
        string namefunction = splitted[i];
        this->i += 2;
        string String = splitted[this->i];
        vector<string> parameters;
        bool CheckWriting = writingFunc || writingWhile;

        this->WriteParameters(splitted, &parameters, CheckWriting, false, &this->i);

        /*for (int j = 0; j < parameters.size(); ++j) {
            cout << parameters[j] << endl;
        }*/

        bool Returning;

        Variable returnedVar = this->executeFunction(namefunction, CheckWriting, parameters, false, &Returning);
        string type2 = returnedVar.get_type();
        if (!Returning)
        {
            this->PrintError("Function returned nothing");
            return;
        }

        if (type2 == "string")
            strvalue = returnedVar.get_str_value();
        else if (type2 == "int")
            strvalue = to_string(returnedVar.get_int_value());
        else if (type2 == "list")
            strvalue = GetListValue(returnedVar);
        else if (type2 == "bool")
            strvalue = to_string(returnedVar.get_bool_value());
        type = type2;
    }
    else if (splitted[this->i] == "[")
    {
        //List
        Variable VAR;
        vector<Variable> LIST = {};
        VAR.setup(name, LIST);
        this->ListWriting.push_back(VAR);
        this->writingList.push_back(true);
        return;
    }
    else if (type != "")
    {
        if (type == "string")
            strvalue = Var.get_str_value();
        else if (type == "int")
            strvalue = to_string(Var.get_int_value());
        else if (type == "bool")
            strvalue = to_string(Var.get_bool_value());
        else if (type == "list")
            strvalue = GetListValue(Var);
    }
    else
        type = getTypeVar(strvalue);

    if (type.empty())
    {
        this->PrintError("Invalid variable");
        return;
    }
    if (splitted.size() > this->i+1 && (splitted[this->i + 1] == "+" || splitted[this->i + 1] == "-"))
    {
        string finalType;
        this->Operation(splitted, &strvalue, &finalType, type, &this->i);
        type = finalType;
    }
    Variable var;
    if (type == "string")
        var.setup(name, strvalue);
    else if (type == "int")
        var.setup(name, stoi(strvalue));
    else if (type == "bool")
    {
        bool VAL;
        if (strvalue == "true")
            VAL = true;
        else
            VAL = false;
        var.setup(name, VAL);
    }
    else if (type == "list")
    {
        int index = 0;
        //this->writingList.push_back(true);
        this->loadList(split(strvalue, ' '), false, &index);

        var.setup(name, ListWriting[0].get_list_value());

        this->ListWriting.erase(ListWriting.end()-1);
    }

    bool found = false;
    for (int j = 0; j < this->variables.size(); j++)
    {
        Variable* variable = &this->variables[j];
        if (variable->get_name() == name)
        {
            *variable = var;
            found = true;
            break;
        }
    }
    if (!found)
    {
        if (!this->Ifs.empty() || this->isExecutingFunc)
        {
            int size = VariablesInfos.size();
            if (this->Ifs.size() > size || this->isExecutingFunc)
            {
                vector<Variable> Vector;
                this->VariablesInfos.push_back(Vector);
            }
            this->VariablesInfos[(this->VariablesInfos.size() - 1)].push_back(var);
        }
        else
            this->variables.push_back(var);
    }
}
Variable Interpreter::loadVariableWithoutWriting(vector<string> splitted, string name)
{
    int j = 0;
    string strvalue = splitted[j];

    string type;

    if (splitted.size() > (j+1) && splitted[j + 1] == "(")
    {
        string namefunction = splitted[j];
        j += 2;
        string String = splitted[j];
        vector<string> parameters;
        bool CheckWriting = writingFunc || writingWhile;
        int num = 0;
        do
        {
            if (j >= splitted.size()) break;

            if (String == ")")
                break;

            if (String != ",")
            {
                if (String == "(")
                {
                    string FunctionName = splitted[j-1];
                    vector<string> params;
                    j++;
                    while (true)
                    {
                        string param = splitted[j];
                        if (param == ")")
                        {
                            num--;
                            break;
                        }
                        if (param != ",")
                            params.push_back(param);
                        if (j >= splitted.size()-1)
                            break;
                        j++;
                    }
                    /*cout << "printing params" << endl;
                    for (int j = 0; j < params.size(); ++j) {
                        cout << params[j] << endl;
                    }*/
                    //cout << "params size = " << params.size() << endl;
                    //executing function
                    Function FUNCTION = this->find_function(FunctionName);
                    if (FUNCTION.get_name() == "")
                    {
                        this->PrintError("Invalid function");
                        Variable var;
                        return var;
                    }
                    else
                    {
                        bool Returning;
                        Variable returned = this->executeFunction(FunctionName, CheckWriting, params, false, &Returning);
                        parameters[parameters.size()-1] = returned.get_value();
                    }
                    //num++;
                }
                else if (num <= 0 && String == ")")
                {
                    if (num <= 0)
                        break;
                }
                else if (String == "=" && splitted[j + 1] == "=")
                {
                    parameters.push_back("==");
                    j++;
                }
                else
                    parameters.push_back(String);
            }

            if (j < splitted.size()-1)
                j++;
            else
                break;
            String = splitted[j];
        } while (true);

        /*for (int j = 0; j < parameters.size(); ++j) {
            cout << parameters[j] << endl;
        }*/

        bool Returning;

        Variable returnedVar = this->executeFunction(namefunction, CheckWriting, parameters, false, &Returning);
        string type2 = returnedVar.get_type();
        if (!Returning)
        {
            this->PrintError("Function returned nothing");
            Variable var;
            return var;
        }
        if (type2 == "string")
            strvalue = returnedVar.get_str_value();
        else if (type2 == "int")
            strvalue = to_string(returnedVar.get_int_value());
        type = type2;
    }
    else if (splitted[j] == "[") {
        //List
        Variable VAR;
        vector<Variable> LIST = {};
        VAR.setup(name, LIST);
        this->ListWriting.push_back(VAR);
        this->writingList.push_back(true);
        return VAR;
    }
    else if (!(this->find_variable(strvalue)).get_type().empty())
    {
        Variable VAR = this->find_variable(strvalue);
        type = VAR.get_type();
        if (type == "string")
            strvalue = VAR.get_str_value();
        else if (type == "int")
            strvalue = to_string(VAR.get_int_value());
        else if (type == "bool")
            strvalue = to_string(VAR.get_bool_value());
        else if (type == "list")
            strvalue = GetListValue(VAR);
    }
    else
        type = getTypeVar(strvalue);

    if (type.empty())
    {
        this->PrintError("Invalid variable");
        Variable var;
        return var;
    }

    if (splitted.size() > j+1 && (splitted[j + 1] == "+" || splitted[j + 1] == "-"))
    {
        string finalType;
        this->Operation(splitted, &strvalue, &finalType, type, &j);
        type = finalType;
    }
    Variable var;
    if (type == "string")
        var.setup(name, strvalue);
    else if (type == "int")
        var.setup(name, stoi(strvalue));
    else if (type == "bool")
    {
        bool VAL;
        if (strvalue == "true")
            VAL = true;
        else
            VAL = false;
        var.setup(name, VAL);
    }
    else if (type == "list")
    {
        int index = 0;
        //this->writingList.push_back(true);
        this->loadList(split(strvalue, ' '), false, &index);

        var.setup(name, ListWriting[0].get_list_value());

        this->ListWriting.erase(ListWriting.end()-1);
    }

    return var;
}

void Interpreter::loadList(vector<string> splitted, bool write, int* index)
{
    //Loading list variable
    for (; *index < splitted.size(); (*index)++) {
        string String = splitted[*index];
        //cout << "String = " << String << ", index = " << *index << ", line = " << this->line << endl;
        if (String == "[")
        {
            //Another list
            bool Bool = listAll.size() >= 2;

            if (Bool)
                this->loadVariableWithoutWriting(listAll, "");
            else
                this->writingList.push_back(true);

            if (((*index) + 1) < splitted.size()-1)
            {
                (*index)++;
                while (true)
                {
                    listAll.push_back(splitted[*index]);
                    (*index)++;
                    if (splitted.size() < *index)
                    {
                        return;
                    }
                    if (splitted[*index] == "]" || splitted[*index] == "," || splitted[*index] == "[")
                        break;
                }
            }

            Variable list;
            vector<Variable> LIST;
            list.setup("", LIST);

            this->ListWriting.push_back(list);
            if (!listAll.empty())
            {
                Variable var2 = this->loadVariableWithoutWriting(listAll, "");
                this->ListWriting[ListWriting.size()-1].add_item_list(var2);
            }
            listAll = {};
        }
        else if (String == ",")
        {
            if (!listAll.empty())
            {
                Variable var = this->loadVariableWithoutWriting(listAll, "");
                this->ListWriting[ListWriting.size() - 1].add_item_list(var);
                listAll = {};
            }
        }
        else if (String == "]")
        {
            //Ending of list/sub-list
            this->writingList.erase(this->writingList.end() - 1);

            if (!listAll.empty())
            {
                Variable var = this->loadVariableWithoutWriting(listAll, "");
                this->ListWriting[ListWriting.size() - 1].add_item_list(var);
                listAll = {};
            }

            if (writingList.empty())
            {
                while (true)
                {
                    if (this->ListWriting.size()-1 > 0)
                    {
                        if (listAll.size() > 0)
                        {
                            Variable var = this->loadVariableWithoutWriting(listAll, "");
                            this->ListWriting[ListWriting.size() - 1].add_item_list(var);
                            listAll = {};
                        }
                        this->ListWriting[ListWriting.size()-2].add_item_list(ListWriting[ListWriting.size()-1]);
                        this->ListWriting.erase(this->ListWriting.end() - 1);
                    }
                    else
                        break;
                }
                if (writingList.empty() && write)
                {
                    //Adding value to variable
                    Variable var = this->ListWriting[0];
                    string name = var.get_name();
                    bool found = false;
                    for (int j = 0; j < this->variables.size(); j++)
                    {
                        Variable* variable = &this->variables[j];
                        if (variable->get_name() == name)
                        {
                            *variable = var;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        if (!this->Ifs.empty() || this->isExecutingFunc)
                        {
                            int size = VariablesInfos.size();
                            if (this->Ifs.size() > size || this->isExecutingFunc)
                            {
                                vector<Variable> Vector;
                                this->VariablesInfos.push_back(Vector);
                            }
                            this->VariablesInfos[(this->VariablesInfos.size() - 1)].push_back(var);
                        }
                        else
                            this->variables.push_back(var);
                    }
                    this->ListWriting.erase(this->ListWriting.end() - 1);
                }
                if (this->writingList.size() == 0)
                    return;
            }
            else
            {
                this->ListWriting[ListWriting.size()-2].add_item_list(ListWriting[ListWriting.size()-1]);
                this->ListWriting.erase(this->ListWriting.end() - 1);
            }
        }
        else
            listAll.push_back(String);
    }
}

//Deprecated
void Interpreter::loadIntVariable(vector<string> splitted, string name)
{
    string typeFinalValue;
    string value = splitted[(++this->i)];
    Variable Var = this->find_variable(value);
    const string type = Var.get_type();
    if (type != "")
    {
        //cout << "e' una variabile" << endl;
        if (type == "string")
            value = Var.get_str_value();
        else if (type == "int")
            value = to_string(Var.get_int_value());
    }
    this->AddIntegers(splitted, &value, &typeFinalValue);
    if (typeFinalValue == "invalid")
    {
        return;
    }
    if (!isNan(string(1, (name[0]))))
    {
        this->PrintError("A variable's name cannot start with a number");
        return;
    }
    bool found = false;
    for (int i = 0; i < this->variables.size(); i++)
    {
        Variable* var = &this->variables[i];
        if (var->get_name() == name)
        {
            var->set_value(stoi(value));
            found = true;
            break;
        }
    }
    if (!found)
    {
        Variable var;
        var.setup(name, stoi(value));
        if (this->Ifs.size() > 0 || this->isExecutingFunc)
        {
            int size = VariablesInfos.size();
            if (this->Ifs.size() > size || this->isExecutingFunc)
            {
                vector<Variable> vettore;
                this->VariablesInfos.push_back(vettore);
            }
            this->VariablesInfos[(this->VariablesInfos.size() - 1)].push_back(var);
        }
        else
            this->variables.push_back(var);
    }
}
void Interpreter::loadStringVariable(vector<string> splitted, string name)
{
    string typeFinalValue;
    string value = splitted[(++this->i)];

    this->AddStrings(splitted, &value, &typeFinalValue);
    if (!isNan(string(1, (name[0]))))
    {
        this->PrintError("A variable's name cannot start with a number");
    }
    else
    {
        bool found = false;
        for (int i = 0; i < this->variables.size(); i++)
        {
            Variable* var = &this->variables[i];
            if (var->get_name() == name)
            {
                var->set_value(value);
                found = true;
                break;
            }
        }
        if (!found)
        {
            Variable var;
            var.setup(name, value);
            if (this->Ifs.size() > 0 || this->isExecutingFunc)
            {
                int size = VariablesInfos.size();
                if (Ifs.size() <= size || this->isExecutingFunc)
                {
                    vector<Variable> vettore;
                    VariablesInfos.push_back(vettore);
                }
                this->VariablesInfos[size].push_back(var);
            }
            else
                this->variables.push_back(var);
        }
    }
}