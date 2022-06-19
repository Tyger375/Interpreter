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

void Interpreter::execute_internal_function(Variable* variable, const string& function_name, vector<string> parameters)
{
    if (variable->get_type() == "list")
    {
        /*cout << "function_name = " << function_name << endl;
        cout << "printing params:" << endl;
        for (int j = 0; j < parameters.size(); ++j)
        {
            cout << "\t" << parameters[j] << endl;
        }*/
        if (function_name == "add")
        {
            this->internal_add(variable, move(parameters));
        }
        else if (function_name == "remove")
        {
            this->internal_remove(variable, move(parameters));
        }
        //else if (function_name == "")
        else
        {
            this->PrintError("Invalid function");
            return;
        }
        //TODO: length
    }
    else
    {
        this->PrintError("Invalid function");
        //*error = true;
    }
}

void Variable::setup(string var_name, int val)
{
	this->IntValue = val;
	this->name = move(var_name);
	this->valueType = "int";
}

void Variable::setup(string var_name, bool val)
{
    this->BoolValue = val;
    this->name = move(var_name);
    this->valueType = "bool";
}

void Variable::setup(string var_name, vector<Variable> val)
{
    this->name = move(var_name);
    this->valueType = "list";
    this->ListValue = move(val);
}

void Variable::setup(string var_name, string val)
{
    /*if (val[0] != '"' && !val.empty())
    {
        //cout << "adding " << '"' << " to " << val << endl;
        //val = '"' + val + '"';
    }*/
	this->StrValue = move(val);
	this->name = move(var_name);
	this->valueType = "string";
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
    string str_value = splitted[(++this->i)];
    Variable Var = this->find_variable(str_value);

    string type = Var.get_type();

    //TODO: internal functions with return value
    if (splitted.size() > (this->i+1) && splitted[this->i + 1] == "(")
    {
        // cout << "function" << endl;
        string name_function = splitted[i];
        this->i += 2;
        string String = splitted[this->i];
        vector<string> parameters;
        bool CheckWriting = writingFunc || (writingWhile.size() > 0);

        this->WriteParameters(splitted, &parameters, CheckWriting, false, &this->i);

        /*for (int j = 0; j < parameters.size(); ++j) {
            cout << parameters[j] << endl;
        }*/

        bool Returning;

        Variable returnedVar = this->executeFunction(name_function, CheckWriting, parameters, false, &Returning);
        string type2 = returnedVar.get_type();
        if (!Returning)
        {
            cout << "here" << endl;
            this->PrintError("Function returned nothing");
            return;
        }

        if (type2 == "string")
            str_value = returnedVar.get_str_value();
        else if (type2 == "int")
            str_value = to_string(returnedVar.get_int_value());
        else if (type2 == "list")
            str_value = GetListValue(returnedVar);
        else if (type2 == "bool")
            str_value = to_string(returnedVar.get_bool_value());
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
    else if (!type.empty())
    {
        if (type == "string")
            str_value = Var.get_str_value();
        else if (type == "int")
            str_value = to_string(Var.get_int_value());
        else if (type == "bool")
            str_value = to_string(Var.get_bool_value());
        else if (type == "list")
            str_value = GetListValue(Var);
    }
    else if (splitted[this->i] == "-")
    {
        type = "int";
        str_value = "-" + splitted[this->i + 1];
    }
    else
        type = getTypeVar(str_value);

    if (type.empty())
    {
        this->PrintError("Invalid variable: " + str_value);
        return;
    }
    if (splitted.size() > this->i+1 && (splitted[this->i + 1] == "+" || splitted[this->i + 1] == "-"))
    {
        string finalType;
        this->Operation(splitted, &str_value, &finalType, type, &this->i);
        type = finalType;
    }
    //TODO: {var} = {list}[{index}]

    Variable var;
    if (type == "string")
    {
        /*if (str_value[0] == '"')// && str_value[1] == '"' && str_value.length() > 2)
        {
            str_value.erase(str_value.begin());
            //str_value.erase(str_value.begin());
            //str_value.erase(str_value.end() - 1);
            str_value.erase(str_value.end() - 1);
        }*/
        //cout << str_value << endl;
        var.setup(name, str_value);
    }
    else if (type == "int")
        var.setup(name, stoi(str_value));
    else if (type == "bool")
    {
        bool VAL;
        if (str_value == "true")
            VAL = true;
        else
            VAL = false;
        var.setup(name, VAL);
    }
    else if (type == "list")
    {
        int index = 0;
        //this->writingList.push_back(true);
        while (!writingList.empty())
        {
            writingList.erase(writingList.end()-1);
        }
        this->loadList(split(str_value, ' '), false, &index);

        var.setup(name, ListWriting[0].get_list_value());

        this->ListWriting.erase(ListWriting.end()-1);
    }

    bool found = false;
    for (auto &j : this->variables)
    {
        Variable* variable = &j;
        if (variable->get_name() == name)
        {
            *variable = var;
            found = true;
            cout << "found here1" << endl;
            cout << var.get_value() << endl;
            break;
        }
    }
    for (int j = 0; j < this->VariablesInfos.size(); j++)
    {
        for (int k = 0; k < VariablesInfos[j].size(); k++)
        {
            Variable* variable = &VariablesInfos[j][k];
            if (variable->get_name() == name)
            {
                *variable = var;
                cout << "found here2" << endl;
                found = true;
                break;
            }
        }
    }
    cout << "found = " << found << endl;
    if (!found)
    {
        if (!this->Ifs.empty() || this->isExecutingFunc)
        {
            unsigned int size = VariablesInfos.size();
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
Variable Interpreter::loadVariableWithoutWriting(vector<string> splitted, const string& name)
{
    int j = 0;
    string str_value = splitted[j];

    string type;

    if (splitted.size() > (j+1) && splitted[j + 1] == "(")
    {
        string name_function = splitted[j];
        j += 2;
        string String = splitted[j];
        vector<string> parameters;
        bool CheckWriting = writingFunc || (writingWhile.size() > 0);
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
                    if (FUNCTION.get_name().empty())
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
                    parameters.emplace_back("==");
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

        Variable returnedVar = this->executeFunction(name_function, CheckWriting, parameters, false, &Returning);
        string type2 = returnedVar.get_type();
        if (!Returning)
        {
            this->PrintError("Function returned nothing");
            Variable var;
            return var;
        }
        if (type2 == "string")
            str_value = returnedVar.get_str_value();
        else if (type2 == "int")
            str_value = to_string(returnedVar.get_int_value());
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
    else if (!(this->find_variable(str_value)).get_type().empty())
    {
        Variable VAR = this->find_variable(str_value);
        type = VAR.get_type();
        if (type == "string")
            str_value = VAR.get_str_value();
        else if (type == "int")
            str_value = to_string(VAR.get_int_value());
        else if (type == "bool")
            str_value = to_string(VAR.get_bool_value());
        else if (type == "list")
            str_value = GetListValue(VAR);
    }
    else if (splitted[j] == "-")
    {
        type = "int";
        str_value = "-" + splitted[j + 1];
    }
    else
        type = getTypeVar(str_value);

    if (type.empty())
    {
        this->PrintError("Invalid variable: " + str_value);
        Variable var;
        return var;
    }

    if (splitted.size() > j+1 && (splitted[j + 1] == "+" || splitted[j + 1] == "-"))
    {
        string finalType;
        this->Operation(splitted, &str_value, &finalType, type, &j);
        type = finalType;
    }
    Variable var;
    if (type == "string")
    {
        if (str_value[0] == '"' && str_value[1] == '"' && str_value.length() > 2)
        {
            str_value.erase(str_value.begin());
            str_value.erase(str_value.begin());
            str_value.erase(str_value.end() - 1);
            str_value.erase(str_value.end() - 1);
        }
        var.setup(name, str_value);
    }
    else if (type == "int")
        var.setup(name, stoi(str_value));
    else if (type == "bool")
    {
        bool VAL;
        if (str_value == "true")
            VAL = true;
        else
            VAL = false;
        var.setup(name, VAL);
    }
    else if (type == "list")
    {
        int index = 0;
        //this->writingList.push_back(true);
        vector<string> Splitted = split(str_value, ' ');
        this->loadList(Splitted, false, &index);

        //cout << GetListValue(ListWriting[0]) << endl;

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
                    if (splitted[*index] == "[")
                    {
                        Variable VAR;
                        vector<Variable> LIST = {};
                        VAR.setup("", LIST);
                        this->ListWriting.push_back(VAR);
                        this->writingList.push_back(true);
                    }
                    else
                        listAll.push_back(splitted[*index]);
                    //cout << "splitted[*index] = " << splitted[*index] << endl;
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
                /*cout << "printing" << endl;
                for (auto word : listAll) {
                    cout << word << endl;
                }
                cout << "ended" << endl;*/
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
                        if (!listAll.empty())
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
                    //cout << GetListValue(var) << endl;
                    string name = var.get_name();
                    bool found = false;
                    for (auto &j : this->variables)
                    {
                        Variable* variable = &j;
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
                            unsigned int size = VariablesInfos.size();
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
                if (this->writingList.empty())
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