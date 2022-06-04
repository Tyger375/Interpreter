#include "../Other/Utilities/Utilities.h"
#include "../Interpreter/Interpreter.h"
#include <iostream>
#include <vector>
#include <string>
#include "Variable.h"

using namespace std;
using namespace Utilities;
using namespace interpreter;

void Variable::setup(string name, int val)
{
	this->IntValue = val;
	this->name = name;
	this->valueType = "int";
}

void Variable::setup(string name, bool val)
{
    this->BoolValue = val;
    this->name = name;
    this->valueType = "bool";
}

void Variable::setup(std::string name, std::vector<Variable> val)
{
    this->name = name;
    this->valueType = "list";
    this->ListValue = val;
}

void Variable::setup(string name, string val)
{
    if (val[0] != '"' && val != "")
    {
        //cout << "adding " << '"' << " to " << val << endl;
        //val = '"' + val + '"';
    }
	this->StrValue = val;
	this->name = name;
	this->valueType = "string";
}

void Variable::set_value(int newval)
{
	this->IntValue = newval;
}

void Variable::set_value(string newval)
{
	this->StrValue = newval;
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
        return string(1, this->BoolValue);
    else
        return "";
}

void Interpreter::loadVariable(vector<string> splitted, string name)
{
    //cout << this->i << endl;
    string strvalue = splitted[(++this->i)];
    Variable Var = this->find_variable(strvalue);

    string type = Var.get_type();
    //cout << type << endl;

    if (splitted.size() > (this->i+1) && splitted[this->i + 1] == "(")
    {
        //cout << "function" << endl;
        string namefunction = splitted[i];
        this->i += 2;
        string String = splitted[this->i];
        vector<string> parameters;
        bool CheckWriting = writingFunc || writingWhile;
        int num = 0;
        do
        {
            if (this->i >= splitted.size()) break;

            if (String == ")")
                break;

            if (String != ",")
            {
                //cout << "word = " << stringcheck << endl;
                if (String == "(")
                {
                    string FunctionName = splitted[i-1];
                    //cout << "function name = " << splitted[i-1] << endl;
                    vector<string> params;
                    this->i++;
                    while (true)
                    {
                        string param = splitted[i];
                        //cout << param << endl;
                        if (param == ")")
                        {
                            num--;
                            break;
                        }
                        if (param != ",")
                            params.push_back(param);
                        if (i >= splitted.size()-1)
                            break;
                        this->i++;
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
                        return;
                    }
                    else
                    {
                        bool Returning;
                        Variable returned = this->executeFunction(FunctionName, CheckWriting, params, false, &Returning);
                        //cout << "value = " << returned.get_value() << endl;
                        parameters[parameters.size()-1] = returned.get_value();
                    }
                    //num++;
                }
                else if (num <= 0 && String == ")")
                {
                    if (num <= 0)
                        break;
                }
                else if (String == "=" && splitted[this->i + 1] == "=")
                {
                    parameters.push_back("==");
                    this->i++;
                }
                else
                    //cout << "adding parameter: " << stringcheck << endl;
                    parameters.push_back(String);
            }

            if (this->i < splitted.size()-1)
                this->i++;
            else
                break;
            String = splitted[this->i];
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
            return;
        }
        //cout << type2 << " " << returnedVar.get_value() << endl;
        if (type2 == "string")
            strvalue = returnedVar.get_str_value();
        else if (type2 == "int")
            strvalue = to_string(returnedVar.get_int_value());
        type = type2;
    }
    else if (splitted[this->i] == "[")
    {
        //cout << "It's a list" << endl;
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
    }
    else
        type = getTypeVar(strvalue);

    if (type == "")
    {
        this->PrintError("Invalid variable");
        return;
    }
    if (splitted.size() > this->i+1 && (splitted[this->i + 1] == "+" || splitted[this->i + 1] == "-"))
    {
        string finalType;
        this->Operation(splitted, &strvalue, &finalType, type, &this->i);
        type = finalType;
        //cout << "ended with: " << strvalue << " " << finalType << endl;
    }
    Variable var;
    if (type == "string")
        var.setup(name, strvalue);
    else if (type == "int")
    {
        //cout << strvalue << endl;
        var.setup(name, stoi(strvalue));
    }
    else if (type == "bool")
    {
        bool VAL;
        if (strvalue == "true")
            VAL = true;
        else
            VAL = false;
        var.setup(name, VAL);
    }

    bool found = false;
    for (int i = 0; i < this->variables.size(); i++)
    {
        Variable* variable = &this->variables[i];
        if (variable->get_name() == name)
        {
            *variable = var;
            found = true;
            break;
        }
    }
    if (!found)
    {
        if (this->Ifs.size() > 0 || this->isExecutingFunc)
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
        //cout << "function" << endl;
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
                //cout << "word = " << stringcheck << endl;
                if (String == "(")
                {
                    string FunctionName = splitted[j-1];
                    //cout << "function name = " << splitted[i-1] << endl;
                    vector<string> params;
                    j++;
                    while (true)
                    {
                        string param = splitted[j];
                        //cout << param << endl;
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
                        //cout << "value = " << returned.get_value() << endl;
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
                    //cout << "adding parameter: " << stringcheck << endl;
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
        //cout << type2 << " " << returnedVar.get_value() << endl;
        if (type2 == "string")
            strvalue = returnedVar.get_str_value();
        else if (type2 == "int")
            strvalue = to_string(returnedVar.get_int_value());
        type = type2;
    }
    else if (splitted[j] == "[") {
        //cout << "It's a list" << endl;
        Variable VAR;
        vector<Variable> LIST = {};
        VAR.setup(name, LIST);
        this->ListWriting.push_back(VAR);
        this->writingList.push_back(true);
        //cout << ListWriting.size() << " " << writingList.size() << endl;
        return VAR;
    }
    else
        type = getTypeVar(strvalue);

    //cout << type << " " << splitted[0] << " " << strvalue << endl;

    if (type == "")
    {
        //cout << "here" << endl;
        this->PrintError("Invalid variable");
        Variable var;
        return var;
    }
    if (splitted.size() > j+1 && (splitted[j + 1] == "+" || splitted[j + 1] == "-"))
    {
        string finalType;
        this->Operation(splitted, &strvalue, &finalType, type, &j);
        type = finalType;
        //cout << "ended with: " << strvalue << " " << finalType << endl;
    }
    Variable var;
    if (type == "string")
        var.setup(name, strvalue);
    else if (type == "int")
    {
        //cout << strvalue << endl;
        var.setup(name, stoi(strvalue));
    }
    else if (type == "bool")
    {
        bool VAL;
        if (strvalue == "true")
            VAL = true;
        else
            VAL = false;
        var.setup(name, VAL);
    }

    return var;
}

void Interpreter::loadList(std::vector<std::string> splitted, bool write, int* index)
{
    vector<string> All = {};
    //cout << *index << " " << index << endl;
    for (; *index < splitted.size(); (*index)++) {
        string String = splitted[*index];
        //cout << String << endl;
        if (String == "[")
        {
            //cout << "it's [" << endl;
            bool Bool = All.size() >= 2;
            if (Bool)
            {
                this->loadVariableWithoutWriting(All, "");
            }

            //cout << (splitted[i] != ",") << " " << (splitted[i] != "]") << " " << (splitted.size() < i) << endl;
            (*index)++;
            while (true)
            {
                All.push_back(splitted[*index]);
                //cout << splitted.size() << " " << *index << endl;
                //cout << (splitted[i] == ",") << " " << (splitted[i] == "]") << " " << (splitted.size() < i) << endl;
                //cout << splitted
                (*index)++;
                if (splitted.size() < *index)
                {
                    return;
                }
                if (splitted[*index] == "]" || splitted[*index] == "," || splitted[*index] == "[")
                    break;
                //cout << (splitted[i] != ",") << " " << (splitted[i] != "]") << " " << (splitted.size() < i) << endl;
            }


            /*cout << "printing All: " << endl;
            for (int j = 0; j < All.size(); ++j) {
                cout << All[j] << endl;
            }
            cout << "Ended" << endl;*/


            Variable var2 = this->loadVariableWithoutWriting(All, "");


            Variable list;
            vector<Variable> LIST;
            list.setup("", LIST);
            this->ListWriting.push_back(list);
            this->ListWriting[ListWriting.size()-1].add_item_list(var2);
            //cout << "ListWriting size = " << ListWriting.size() << endl;
            //String = splitted[*index];
            All = {};
        }
        else if (String == "]")
        {
            this->writingList.erase(this->writingList.end() - 1);

            if (ListWriting.size() == 1)
            {
                if (All.size() > 0)
                {
                    Variable var = this->loadVariableWithoutWriting(All, "");
                    this->ListWriting[ListWriting.size() - 1].add_item_list(var);
                    All = {};
                }
            }

            while (true)
            {
                if (this->ListWriting.size()-1 > 0)
                {
                    if (All.size() > 0)
                    {
                        Variable var = this->loadVariableWithoutWriting(All, "");
                        this->ListWriting[ListWriting.size() - 1].add_item_list(var);
                        All = {};
                    }
                    this->ListWriting[ListWriting.size()-2].add_item_list(ListWriting[ListWriting.size()-1]);
                    /*if (this->ListWriting.size()-2 <= 0)
                    {
                        if (write)
                        {
                            cout << "deleting" << endl;
                            this->ListWriting.erase(this->ListWriting.end() - 1);
                        }
                    }
                    else*/
                    this->ListWriting.erase(this->ListWriting.end() - 1);
                }
                else
                    break;
            }
            //cout << ListWriting.size() << " " << write << endl;
            if (writingList.size() == 0 && write)
            {
                //this->printList(this->ListWriting[0]);
                //cout << endl;
                Variable var = this->ListWriting[0];
                string name = var.get_name();
                //cout << "name = " << name << endl;
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
                    if (this->Ifs.size() > 0 || this->isExecutingFunc)
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
                //cout << var.get_type() << endl;
                this->ListWriting.erase(this->ListWriting.end() - 1);
            }
            if (this->writingList.size() == 0)
                return;
        }
        else if (String == ",")
        {
            if (All.size() > 0)
            {
                Variable var = this->loadVariableWithoutWriting(All, "");
                this->ListWriting[ListWriting.size() - 1].add_item_list(var);
                //cout << "Added variable with type = " << var.get_type() << endl;
                All = {};
            }
        }
        else
        {
            //cout << "Adding = " << String << endl;
            All.push_back(String);
        }
        //cout << String << endl;
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
    //cout << valore << " e' il valore della variabile" << endl;
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
            //cout << size << " " << Ifs.size() << endl;
            if (this->Ifs.size() > size || this->isExecutingFunc)
            {
                //cout << "ok" << endl;
                vector<Variable> vettore;
                this->VariablesInfos.push_back(vettore);
            }
            //cout << this->VariablesInfos.size() << " " << Ifs.size() << endl;
            this->VariablesInfos[(this->VariablesInfos.size() - 1)].push_back(var);
            //cout << "F" << endl;
        }
        else
            this->variables.push_back(var);
    }

    //cout << "Output: " << var.get_int_value() << endl;
}
void Interpreter::loadStringVariable(vector<string> splitted, string name)
{
    string typeFinalValue;
    string value = splitted[(++this->i)];

    this->AddStrings(splitted, &value, &typeFinalValue);
    //cout << valore << " e' il valore della variabile" << endl;
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

    //cout << "Output: " << var.get_int_value() << endl;
}