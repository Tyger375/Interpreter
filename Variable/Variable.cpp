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

Variable Interpreter::execute_internal_function(Variable* variable, const string& function_name, vector<string> parameters)
{
    if (variable->get_type() == "list")
    {
        /*cout << "function_name = " << function_name << endl;
        cout << "printing params:" << endl;
        for (int j = 0; j < parameters.size(); ++j)
        {
            cout << "\t" << parameters[j] << endl;
        }*/
        Variable returnedVar;
        if (function_name == "add")
        {
            returnedVar = this->internal_add(variable, move(parameters));
        }
        else if (function_name == "remove")
        {
            returnedVar = this->internal_remove(variable, move(parameters));
        }
        else if (function_name == "length")
        {
            returnedVar = this->internal_length(variable);
        }
        else if (function_name == "contains")
        {
            returnedVar = this->internal_contains(variable, move(parameters));
        }
        else
        {
            this->PrintError("Invalid function");
            Variable var;
            return var;
        }
        return returnedVar;
    }
    else if (variable->get_type() == "string")
    {
        Variable returnedVar;
        if (function_name == "length")
        {
            returnedVar = this->internal_length(variable);
        }
        else if (function_name == "split")
        {
            returnedVar = this->internal_split(variable, move(parameters));
        }
        else if (function_name == "low")
        {
            returnedVar = this->internal_lower(variable);
        }
        else if (function_name == "up")
        {
            returnedVar = this->internal_upper(variable);
        }
        else if (function_name == "remove")
        {
            returnedVar = this->internal_remove(variable, move(parameters));
        }
        else
        {
            this->PrintError("Invalid function");
            Variable var;
            return var;
        }
        return returnedVar;
    }
    else
    {
        this->PrintError("Invalid function");
        Variable var;
        return var;
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

void Variable::setup(string var_name, map<string, Variable> val)
{
    this->name = move(var_name);
    this->valueType = "dict";
    this->DictValue = move(val);
}

void Variable::setup(string var_name, string val)
{
    val = '"' + val + '"';
	this->StrValue = move(val);
	this->name = move(var_name);
	this->valueType = "string";
}

string Variable::get_value()
{
    if (this->valueType == "string")
        return this->StrValue;
    else if (this->valueType == "int")
        return to_string(this->IntValue);
    else if (this->valueType == "bool")
        return to_string(this->BoolValue);
    else if (this->valueType == "list")
        return GetListValue(*this);
    else if (this->valueType == "dict")
        return GetDictValue(*this);
    else
        return "";
}

void Interpreter::InternalFunctionLoadVariable(string* str_value, string* type, vector<string> splitted, string name, int* Index)
{
    Variable* VARIABLE;

    Variable OriginList;
    Variable *List = &OriginList;

    bool with_index = false;
    string lastString = splitted[*Index];
    string local_name = lastString;

    if (lastString == "]")
    {
        with_index = true;
        //accessing member
        bool found = false;
        int index = *Index;
        vector<string> indexes;
        do
        {
            //cout << splitted[index-1] << endl;
            if (lastString == "]")
            {
                //index--;
                if (isNan(splitted[index - 1]))
                {
                    this->PrintError("Invalid index");
                }
                else
                {
                    //index--;
                    while (true)
                    {
                        index--;
                        if (index < 0)
                        {
                            this->PrintError("Invalid syntax");
                            return;
                        }
                        if (splitted[index] == "[")
                        {
                            indexes.push_back(splitted[index + 1]);
                            if (splitted[index - 1] != "]")
                                found = true;
                            break;
                        }
                    }
                }
            }
            if (found)
            {
                local_name = splitted[index - 1];
                break;
            }
        } while (!found);

        Variable VAR = find_variable(local_name);
        string ListValue = GetListValue(VAR);

        vector<string> Splitted = split(ListValue, ' ');

        int index2 = 0;
        //this->writingList.push_back(true);
        this->loadList(Splitted, false, &index2);

        OriginList = this->ListWriting[0];
        Variable ListToWrite = OriginList;

        //this->ListWriting.erase(ListWriting.end()-1);

        i++;

        for (int j = int(indexes.size()) - 1; j >= 0; --j)
        {
            //member inside a list
            string str_Index = indexes[j];
            if (str_Index == "]")
            {
                this->PrintError("No index");
                return;
            }
            if (getTypeVar(str_Index) != "int")
            {
                this->PrintError("Invalid index type");
                return;
            }
            index++;

            if (List->get_list_value().size() <= stoi(str_Index))
            {
                this->PrintError("Index out of range");
                return;
            }
            Variable member = List->get_list_value()[stoi(str_Index)];
            string type = member.get_type();
            {
                if (type == "list" || type == "string")
                {
                    Variable *Pointer = List->get_item_list_pointer(stoi(str_Index));
                    List = Pointer;
                }
                else
                {
                    this->PrintError("Invalid syntax");
                    return;
                }
            }
        }
        while (!this->ListWriting.empty())
        {
            this->ListWriting.erase(ListWriting.end() - 1);
        }
        while (!this->writingList.empty())
        {
            this->writingList.erase(writingList.end() - 1);
        }
        VARIABLE = List;
    }
    else
    {
        Variable Finding = find_variable(local_name);
        (*Index)++;
        if (Finding.get_type().empty())
        {
            //cout << "test" << endl;
            this->PrintError("Invalid variable: " + local_name);
            return;
        }
        VARIABLE = this->find_variable_pointer(local_name);
    }

    if (*Index-1 >= 0)
    {
        if (*Index < splitted.size()-1)
        {
            string function_name = splitted[*Index + 1];
            if (i+1 < splitted.size()-1 && splitted[*Index+2] == "(")
            {
                bool CheckWriting = writingFunc || (!writingWhile.empty()) || writingComment;
                vector<string> parameters;
                (*Index) += 3;
                this->WriteParameters(splitted, &parameters, CheckWriting, false, Index);
                Variable returning = this->execute_internal_function(VARIABLE, function_name, parameters);

                if (error)
                {
                    //this->PrintError("Error");
                    return;
                }
                //cout << "returned " << returning.get_value() << " " << returning.get_type() << endl;
                //cout << returning.get_list_value()[0].get_value() << endl;
                /*cout << "Origin list = ";
                this->printList(OriginList);
                cout << endl;*/

                if (with_index)
                {
                    Variable newVar;
                    newVar.setup(local_name, OriginList.get_list_value());
                    *this->find_variable_pointer(local_name) = newVar;
                }

                *str_value = returning.get_value();
                *type = returning.get_type();
            }
            /*else
            {
                Accessing variable
            }*/
        }
        else
        {
            this->PrintError("Invalid syntax");
        }
    }
    else
    {
        this->PrintError("Invalid variable");
    }
}

void Interpreter::loadVariable(vector<string> splitted, const string& name)
{
    //cout << this->i << endl;
    string str_value = splitted[(++this->i)];
    Variable Var = this->find_variable(str_value);

    string type = Var.get_type();

    if (splitted.size() > (this->i+1) && splitted[this->i + 1] == "(")
    {
        // cout << "function" << endl;
        string name_function = splitted[i];
        this->i += 2;
        string String = splitted[this->i];
        vector<string> parameters;
        bool CheckWriting = writingFunc || (!writingWhile.empty());

        this->WriteParameters(splitted, &parameters, CheckWriting, false, &this->i);

        /*for (int j = 0; j < parameters.size(); ++j) {
            cout << parameters[j] << endl;
        }*/

        bool Returning = true;

        Variable returnedVar = this->executeFunction(name_function, CheckWriting, parameters, false, &Returning);
        string type2 = returnedVar.get_type();
        if (!Returning)
        {
            //cout << "here" << endl;
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
    else if (splitted.size() > (this->i+1) && splitted[this->i + 1] == ".")
    {
        //cout << "in-variable function" << endl;
        this->InternalFunctionLoadVariable(&str_value, &type, splitted, name, &this->i);
    }
    else if (splitted.size() > (this->i+1) && splitted[i+1] == "[")
    {
        if (splitted[i] != "[")
        {
            while (true)
            {
                if (this->i == splitted.size()-1)
                    break;
                if (splitted[i] == "]")
                {
                    if (i+1 < splitted.size()-1 && splitted[i+1] != "[")
                        break;
                }
                i++;
            }
            if (i+1 < splitted.size()-1)
                this->InternalFunctionLoadVariable(&str_value, &type, splitted, name, &this->i);
            else
            {
                Variable* VARIABLE;

                Variable OriginList;
                Variable *List = &OriginList;

                string lastString = splitted[i];
                int index = i;
                if (lastString == " ")
                {
                    lastString = splitted[i - 1];
                    index = i-1;
                }

                bool found = false;
                vector<string> indexes;
                do
                {
                    if (lastString == "]")
                    {
                        //index--;
                        if (isNan(splitted[index - 1]))
                        {
                            this->PrintError("Invalid index");
                        }
                        else
                        {
                            //index--;
                            while (true)
                            {
                                index--;
                                if (index < 0)
                                {
                                    this->PrintError("Invalid syntax");
                                    return;
                                }
                                if (splitted[index] == "[")
                                {
                                    indexes.push_back(splitted[index + 1]);
                                    if (splitted[index - 1] != "]")
                                        found = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (found)
                    {
                        break;
                    }
                } while (!found);

                Variable VAR = find_variable(str_value);

                bool IsString = false;
                if (VAR.get_type() == "list")
                {
                    string ListValue = GetListValue(VAR);

                    vector<string> Splitted = split(ListValue, ' ');

                    int index2 = 0;
                    //this->writingList.push_back(true);
                    this->loadList(Splitted, false, &index2);
                    OriginList = this->ListWriting[0];
                }
                else if (VAR.get_type() == "string")
                {
                    IsString = true;
                    vector<Variable> chars;

                    string StrValue = VAR.get_str_value();
                    StrValue.erase(StrValue.begin());
                    StrValue.erase(StrValue.end()-1);

                    for (char character : StrValue)
                    {
                        Variable char_var;
                        char_var.setup("", string(1, character));
                        chars.push_back(char_var);
                    }
                    OriginList.setup("", chars);
                }

                Variable ListToWrite = OriginList;
                i++;
                for (int j = int(indexes.size()) - 1; j >= 0; --j)
                {
                    //member inside a list
                    string str_Index = indexes[j];
                    if (str_Index == "]")
                    {
                        this->PrintError("No index");
                        return;
                    }
                    if (getTypeVar(str_Index) != "int")
                    {
                        this->PrintError("Invalid index local_type");
                        return;
                    }
                    index++;

                    Variable member;
                    if (List->get_type() == "list")
                    {
                        if (List->get_list_value().size() <= stoi(str_Index))
                        {
                            this->PrintError("Index out of range");
                            return;
                        }
                        member = List->get_list_value()[stoi(str_Index)];
                    }
                    else if (List->get_type() == "string")
                    {
                        if (List->get_str_value().size() <= stoi(str_Index)) {
                            this->PrintError("Index out of range");
                            return;
                        }
                        member.setup("", string(1, List->get_str_value()[stoi(str_Index)]));
                    }
                    string local_type = member.get_type();
                    if (local_type == "list" || local_type == "string")
                    {
                        Variable *Pointer = List->get_item_list_pointer(stoi(str_Index));
                        List = Pointer;
                    }
                    else
                    {
                        this->PrintError("Invalid syntax");
                        return;
                    }
                }
                while (!this->ListWriting.empty())
                {
                    this->ListWriting.erase(ListWriting.end() - 1);
                }
                while (!this->writingList.empty())
                {
                    this->writingList.erase(writingList.end() - 1);
                }
                VARIABLE = List;

                type = VARIABLE->get_type();
                if (type == "string")
                    str_value = VARIABLE->get_str_value();
                else if (type == "int")
                    str_value = to_string(VARIABLE->get_int_value());
                else if (type == "bool")
                    str_value = to_string(VARIABLE->get_bool_value());
                else if (type == "list")
                    str_value = GetListValue(*VARIABLE);
            }
        }
    }
    else if (splitted[this->i] == "[")
    {
        //List
        Variable VAR;
        vector<Variable> LIST = {};
        VAR.setup(name, LIST);
        this->ListWriting.push_back(VAR);
        this->writingList.push_back(true);
        if (writingList.size() == 1)
        {
            this->mainWriting = "list";
        }
        return;
    }
    else if (splitted[this->i] == "{")
    {
        //Dictionary
        Variable VAR;
        map<string, Variable> DICT = {};
        VAR.setup(name, DICT);
        this->DictWriting.push_back(VAR);
        this->writingDict.push_back(true);
        if (writingDict.size() == 1)
        {
            this->mainWriting = "dict";
        }
        this->writingKey = true;
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
        this->PrintError("Invalid variable for assignment: " + str_value);
        return;
    }
    if (splitted.size() > this->i+1 && (splitted[this->i + 1] == "+" || splitted[this->i + 1] == "-"))
    {
        string finalType;
        this->Operation(splitted, &str_value, &finalType, type, &this->i);
        type = finalType;
    }

    Variable var;
    if (type == "string")
    {
        while (str_value[0] == '"')
        {
            str_value.erase(str_value.begin());
            str_value.erase(str_value.end() - 1);
        }
        var.setup(name, str_value);
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

        while (!writingList.empty())
        {
            writingList.erase(writingList.end()-1);
        }

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
            //cout << "adding to variables: " << variable->get_name() << " " << variable->get_value() << endl;
            found = true;
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
                //cout << "size = " << VariablesInfos.size() << endl;
                //cout << "adding to VariablesInfos: " << variable->get_name() << " " << variable->get_value() << endl;
                found = true;
                break;
            }
        }
    }

    //cout << this->line << " found " << name << " " << found << ", assigned " << var.get_value() << endl;

    if (!found)
    {
        if (!this->Ifs.empty() || this->isExecutingFunc)
        {
            //unsigned int size = VariablesInfos.size();
            //cout << VariablesInfos.size() << endl;
            /*if (this->Ifs.size() > size || this->isExecutingFunc)
            {
                vector<Variable> Vector;
                this->VariablesInfos.push_back(Vector);
            }*/
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

    //cout << "test heheheha" << endl;
    /*for (auto string : splitted) {
        cout << string << endl;
    }*/
    if (splitted.size() > (j+1) && splitted[j + 1] == "(")
    {
        string name_function = splitted[j];
        j += 2;
        string String = splitted[j];
        vector<string> parameters;
        bool CheckWriting = writingFunc || (!writingWhile.empty());
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

        bool Returning = true;

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
    else if (splitted.size() > (this->i+1) && splitted[this->i + 1] == ".")
    {
        //cout << "in-variable function" << endl;
        this->InternalFunctionLoadVariable(&str_value, &type, splitted, name, &this->i);
    }
    else if (splitted.size() > (this->i+1) && splitted[i+1] == "[" && splitted[i] != "[")
    {
        while (true)
        {
            if (this->i == splitted.size()-1)
                break;
            if (splitted[i] == "]")
            {
                if (i+1 < splitted.size()-1 && splitted[i+1] != "[")
                    break;
            }
            i++;
        }
        //i++;
        this->InternalFunctionLoadVariable(&str_value, &type, splitted, name, &this->i);
    }
    else if (splitted[j] == "[")
    {
        //List

        Variable VAR;
        vector<Variable> LIST = {};
        VAR.setup(name, LIST);
        this->ListWriting.push_back(VAR);
        this->writingList.push_back(true);
        return VAR;
    }
    else if (splitted[j][0] == '{') {
        //cout << "Dictionary?" << endl;
        str_value = splitted[j];
        type = "dict";
    }
    else if (!(this->find_variable(str_value)).get_type().empty())
    {
        Variable VAR = this->find_variable(str_value);
        type = VAR.get_type();
        /*for (auto string : splitted) {
            cout << string << endl;
        }
        cout << VAR.get_value() << " " << type << " " << i << " " << splitted.size() << endl;*/
        if (type == "string")
            str_value = VAR.get_str_value();
        else if (type == "int")
            str_value = to_string(VAR.get_int_value());
        else if (type == "bool")
            str_value = to_string(VAR.get_bool_value());
        else if (type == "list")
            str_value = GetListValue(VAR);
        else if (type == "dict")
            str_value = GetDictValue(VAR);
        /*
        cout << "str_value = " << str_value << endl;
        cout << type << line << endl;
        */
    }
    else if (splitted[j] == "-")
    {
        type = "int";
        str_value = "-" + splitted[j + 1];
    }
    else
        type = getTypeVar(str_value);

    //cout << type << line << endl;
    //cout << splitted[j] << endl;

    if (type.empty())
    {
        string MsgFunction;
        if (isExecutingFunc)
        {
            MsgFunction = "in function '" + this->FUNC->get_name() + "'";
        }
        //cout << str_value << " " << type << " " << i << " " << splitted.size() << " " << this->line << endl;
        this->PrintError("Invalid variable while reading " + MsgFunction + ": " + str_value);
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
        while (str_value[0] == '"')
        {
            if (str_value.size() == 2)
                break;
            str_value.erase(str_value.begin());
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
        //cout << "loading list" << endl;
        int index = 0;
        //this->writingList.push_back(true);
        vector<string> Splitted = split(str_value, ' ');
        this->loadList(Splitted, false, &index);

        //cout << GetListValue(ListWriting[0]) << endl;

        var.setup(name, ListWriting[0].get_list_value());

        this->ListWriting.erase(ListWriting.end()-1);
    }
    else if (type == "dict")
    {
        int index = 0;
        vector<string> Splitted = split(str_value, ' ');
        Variable VAR;
        map<string, Variable> DICT = {};
        VAR.setup("", DICT);
        this->DictWriting.push_back(VAR);
        this->writingDict.push_back(true);

        Splitted.erase(Splitted.begin());

        this->loadDict(Splitted, false, &index);

        var.setup(name, DictWriting[0].get_dict_value());

        this->DictWriting.erase(DictWriting.end()-1);
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
                /*
                cout << "printing" << endl;
                for (auto word : listAll) {
                    cout << word << endl;
                }
                cout << "ended" << endl;
                */
                if (listAll[0] == "{")
                {
                    Variable VAR;
                    map<string, Variable> DICT = {};
                    VAR.setup("", DICT);
                    this->DictWriting.push_back(VAR);
                    this->writingDict.push_back(true);

                    listAll.erase(listAll.begin());

                    int in = 0;
                    loadDict(listAll, false, &in);

                    Variable returned = DictWriting[DictWriting.size()-1];
                    //this->printDict(returned);
                    //cout << endl;
                    this->ListWriting[ListWriting.size()-1].add_item_list(returned);
                    DictWriting.erase(DictWriting.end()-1);
                }
                else
                {
                    Variable var2 = this->loadVariableWithoutWriting(listAll, "");
                    this->ListWriting[ListWriting.size()-1].add_item_list(var2);
                }
            }
            listAll = {};
        }
        else if (String == ",")
        {
            if (!listAll.empty())
            {
                if (listAll[0] == "{")
                {
                    //cout << DictAll.size() << endl;
                    if (!DictAll.empty())
                        LoadDictItem(write);
                    //cout << "Printing dict" << endl;
                    //this->printDict(DictWriting[DictWriting.size()-1]);
                    //cout << endl;
                }
                else
                {
                    Variable var = this->loadVariableWithoutWriting(listAll, "");
                    this->ListWriting[ListWriting.size() - 1].add_item_list(var);
                    listAll = {};
                }
            }
        }
        else if (String == "]")
        {
            //Ending of list/sub-list
            this->writingList.erase(this->writingList.end() - 1);

            if (!listAll.empty())
            {
                //cout << "here" << endl;
                if (listAll[0] == "{")
                {
                    int Index = 0;
                    listAll.erase(listAll.begin());
                    this->writingDict.push_back(true);
                    Variable test;
                    test.setup("", map<string, Variable>());
                    DictWriting.push_back(test);
                    loadDict(listAll, false, &Index);

                    this->ListWriting[ListWriting.size() - 1].add_item_list(DictWriting[DictWriting.size()-1]);
                    DictWriting.erase(DictWriting.end()-1);
                    listAll = {};
                }
                else
                {
                    Variable var = this->loadVariableWithoutWriting(listAll, "");
                    this->ListWriting[ListWriting.size() - 1].add_item_list(var);
                    listAll = {};
                }
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
                    //cout << "GetListValue = " << GetListValue(var) << endl;
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
                            /*if (this->Ifs.size() > size || this->isExecutingFunc)
                            {
                                vector<Variable> Vector;
                                this->VariablesInfos.push_back(Vector);
                            }*/
                            this->VariablesInfos[(this->VariablesInfos.size() - 1)].push_back(var);
                        }
                        else
                            this->variables.push_back(var);
                    }
                    //cout << GetListValue(find_variable(var.get_name())) << endl;
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

void Interpreter::LoadDictItem(bool write)
{
    if (DictAll[0] == "[")
    {
        int Index = 0;
        this->loadList(DictAll, false, &Index);
        //this->printList(ListWriting[0]);
        this->DictWriting[DictWriting.size()-1].add_item_dict(Keys[Keys.size()-1], ListWriting[0]);
        //cout << endl;
        while (!this->ListWriting.empty())
        {
            this->ListWriting.erase(ListWriting.end()-1);
        }
        while (!this->writingList.empty())
        {
            this->writingList.erase(writingList.end()-1);
        }
    }
    else if (DictAll[0] == "{")
    {
        DictAll.erase(DictAll.begin());
        int Index = 0;

        /*
        for (auto word : DictAll)
        {
            cout << "\t" << word << endl;
        }
        cout << "Starting" << endl;
        */

        map<string, Variable> newMap;
        Variable test;
        test.setup("", newMap);
        this->DictWriting.push_back(test);

        //DictAll.insert(DictAll.begin(), "{");

        this->loadDict(DictAll, false, &Index);
        Variable returnedDict = DictWriting[DictWriting.size()-1];

        /*
        printDict(returnedDict);
        cout << endl << Keys[Keys.size()-1] << endl;
        */
        DictWriting.erase(DictWriting.end()-1);
        DictWriting[DictWriting.size()-1].add_item_dict(Keys[Keys.size()-1], returnedDict);

        /*
        cout << "ended" << endl;
        cout << DictWriting.size() << endl;
        cout << writingDict.size() << endl;
        */

        if (writingDict.size() == 1 && DictWriting.size() == 1)
        {
            /*
            this->printDict(DictWriting[0]);
            cout << endl;
            */
            if (write)
            {
                //cout << "Writing variable" << endl;

                Variable var = this->DictWriting[0];
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
                        this->VariablesInfos[(this->VariablesInfos.size() - 1)].push_back(var);
                    }
                    else
                        this->variables.push_back(var);
                }
                DictWriting.erase(DictWriting.end()-1);
            }
            writingDict.erase(writingDict.end()-1);
            mainWriting = "";
        }
    }
    else
    {
        Variable returned = this->loadVariableWithoutWriting(DictAll, "");
        this->DictWriting[DictWriting.size()-1].add_item_dict(Keys[Keys.size()-1], returned);
    }
    DictAll.clear();
    Keys.erase(Keys.end()-1);
}

void Interpreter::loadDict(vector<string> splitted, bool write, int* index)
{
    for (; *index < splitted.size(); (*index)++) {
        string String = splitted[*index];

        if (String == ":" && this->writingDict.size() == 1)
        {
            writingKey = false;
            Variable returned = this->loadVariableWithoutWriting(DictAll, "");
            string type = returned.get_type();
            if (type == "string")
            {
                Keys.push_back(returned.get_str_value());
            }
            else
            {
                this->PrintError(("Invalid key " + string("'") + String + "'" + " for dictionary, it must be a string"));
                return;
            }
            DictAll.clear();
        }
        else if (String == "{")
        {
            writingDict.push_back(true);
            DictAll.push_back(String);
        }
        else if (String == "}" && this->writingDict.size() > 1)
        {
            writingDict.erase(writingDict.end()-1);
            DictAll.push_back(String);
            if (writingDict.size() == 1)
                LoadDictItem(write);
        }
        else if (String == "[")
        {
            this->writingList.push_back(true);
            DictAll.push_back(String);
        }
        else if (String == "]")
        {
            this->writingList.erase(writingList.end()-1);
            DictAll.push_back(String);
            if (writingList.empty() && this->writingDict.size() == 1)
            {
                LoadDictItem(write);
            }
        }
        else if (String == "," && this->writingList.empty() && this->writingDict.size() == 1)
        {
            /*for (auto string : DictAll)
            {
                cout << string << " ";
            }
            cout << endl;*/
            if (!DictAll.empty())
            {
                LoadDictItem(write);
            }
        }
        else if (String == "}")
        {
            //cout << DictAll.size() << endl;
            if (!DictAll.empty())
                LoadDictItem(write);
            mainWriting = "";
            //cout << "test = " << (writingDict.size()) << DictWriting.size() << endl;
            if (writingDict.size() <= 1 && DictWriting.size() == 1)
            {
                if (write)
                {
                    Variable var = this->DictWriting[0];
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
                            /*if (this->Ifs.size() > size || this->isExecutingFunc)
                            {
                                vector<Variable> Vector;
                                this->VariablesInfos.push_back(Vector);
                            }*/
                            this->VariablesInfos[(this->VariablesInfos.size() - 1)].push_back(var);
                        }
                        else
                            this->variables.push_back(var);
                    }
                    DictWriting.erase(DictWriting.end()-1);
                }
                if (!writingDict.empty())
                    writingDict.erase(writingDict.end()-1);
                mainWriting = "";
            }
        }
        else
        {
            DictAll.push_back(String);
        }
    }
}
