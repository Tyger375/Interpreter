#include <string>
#include "Variable.h"
#include "../Interpreter/Interpreter.h"
#include "../Other/Utilities/Utilities.h"

using namespace std;
using namespace interpreter;
using namespace Utilities;

//Lists
Variable Interpreter::internal_add(Variable* variable, vector<string> parameters)
{
    if (parameters.empty())
    {
        //*Error = true;
        this->PrintError("No parameters given");
        Variable var;
        return var;
    }

    Variable var;
    if (parameters[0][0] == '[')
    {
        int index = 0;
        this->loadList(split(parameters[0], ' '), false, &index);

        var = ListWriting[0];
        this->ListWriting.erase(ListWriting.end()-1);
    }
    else
        var = this->loadVariableWithoutWriting(parameters, "");

    variable->add_item_list(var);
    Variable returningVar;
    return returningVar;
}

Variable Interpreter::internal_contains(Variable* variable, vector<string> parameters)
{
    if (parameters.empty())
    {
        this->PrintError("No parameters given");
        Variable var;
        return var;
    }

    Variable var_param = this->loadVariableWithoutWriting(split(parameters[0], ' '), "");

    vector<Variable> list = variable->get_list_value();

    string type = var_param.get_type();

    bool Found = false;

    for (auto member : list)
    {
        if (member.get_type() != type)
            continue;

        if (type == "string")
        {
            if (member.get_str_value() == var_param.get_str_value())
                Found = true;
        }
        else if (type == "int")
        {
            if (member.get_int_value() == var_param.get_int_value())
                Found = true;
        }
        else if (type == "bool")
        {
            if (member.get_bool_value() == var_param.get_bool_value())
                Found = true;
        }
        else if (type == "list")
        {
            if (GetListValue(member) == GetListValue(var_param))
                Found = true;
        }
        if (Found)
            break;
    }

    Variable returning;
    returning.setup("", Found);

    return returning;
}

//Strings
Variable Interpreter::internal_split(Variable* variable, vector<string> parameters)
{
    if (parameters.empty())
    {
        this->PrintError("No parameters given");
        Variable var;
        return var;
    }

    string separator = parameters[0];

    if (getTypeVar(separator) != "string")
    {
        this->PrintError("Invalid type of separator, it must be a string");
        Variable var;
        return var;
    }

    separator.erase(separator.begin());
    separator.erase(separator.end()-1);

    if (separator.length() > 1)
    {
        this->PrintError("Separator must be a single character");
        Variable var;
        return var;
    }

    char splitter = separator[0];

    string String = variable->get_str_value();

    String.erase(String.begin());
    String.erase(String.end() - 1);

    string word;
    vector<string> words;

    for (int j = 0; j < (String.length()); j++)
    {
        char character = String[j];
        if (((character == splitter) || j == String.length() - 1))
        {
            //cout << word << endl;
            if (character == splitter && !word.empty()) {
                words.push_back(word);
                word = "";
            }
            else if (j == String.length() - 1)
            {
                word += character;
                words.push_back(word);
                word = "";
            }
        }
        else
        {
            string char_to_string = string(1, character);
            if (character != '\t')
                word += character;
        }
    }

    string str_list = "[";

    for (int j = 0; j < words.size(); ++j) {
        string _word = words[j];
        Variable var_word;
        var_word.setup("", _word);
        str_list += var_word.get_str_value();
        if (j != words.size()-1)
        {
            str_list += ",";
        }
    }
    str_list += "]";

    int index = 0;

    this->loadList(split(str_list, ' '), false, &index);

    Variable var = ListWriting[0];

    this->ListWriting.erase(ListWriting.end()-1);

    while (!this->writingList.empty())
    {
        this->writingList.erase(writingList.end()-1);
    }
    return var;
}

Variable Interpreter::internal_lower(Variable* variable)
{
    Variable var;
    string OldString = variable->get_str_value();
    OldString.erase(OldString.begin());
    OldString.erase(OldString.end()-1);
    string NewString;
    for (auto character : OldString)
    {
        NewString += tolower(character);
    }
    //cout << NewString << endl;
    var.setup("", NewString);
    return var;
}

Variable Interpreter::internal_upper(Variable* variable)
{
    Variable var;
    string OldString = variable->get_str_value();
    OldString.erase(OldString.begin());
    OldString.erase(OldString.end()-1);
    string NewString;
    for (auto character : OldString)
    {
        NewString += toupper(character);
    }
    var.setup("", NewString);
    return var;
}

//Dicts
Variable Interpreter::internal_keys(Variable* variable)
{
    map<string, Variable> dict = variable->get_dict_value();
    vector<Variable> keys;
    Variable returning;
    for(auto it = dict.begin(); it != dict.end(); ++it) {
        Variable var;
        var.setup("", it->first);
        keys.push_back(var);
    }
    returning.setup("", keys);
    return returning;
}

//Multiple
Variable Interpreter::internal_length(Variable* variable)
{
    string type = variable->get_type();
    Variable var;
    if (type == "list")
    {
        var.setup("", int(variable->get_list_value().size()));
    }
    else if (type == "string")
    {
        var.setup("", int(variable->get_str_value().length()-2));
    }
    else
    {
        this->PrintError("Invalid type for function: 'length'");
    }
    return var;
}

Variable Interpreter::internal_remove(Variable* variable, vector<string> parameters)
{
    if (parameters.empty())
    {
        this->PrintError("No parameters given");
        Variable var;
        return var;
    }

    if (isNan(parameters[0]))
    {
        this->PrintError("Invalid index");
        Variable var;
        return var;
    }

    int index = stoi(parameters[0]);

    if (variable->get_type() == "list")
    {
        vector<Variable>* ListValue = variable->get_list_value_pointer();

        if (index >= 0)
        {
            if (index > ListValue->size()-1)
            {
                this->PrintError("Index out of range");
                Variable var;
                return var;
            }
            ListValue->erase((ListValue->begin() + index));
        }
        else
        {
            if (index < ListValue->size()-1)
            {
                this->PrintError("Index out of range");
                Variable var;
                return var;
            }
            ListValue->erase((ListValue->end() + index));
        }
    }
    else if (variable->get_type() == "string")
    {
        string* StrValue = variable->get_str_value_pointer();

        if (index >= 0)
        {
            if (index > StrValue->size()-1)
            {
                this->PrintError("Index out of range");
                Variable var;
                return var;
            }
            StrValue->erase((StrValue->begin() + index));
        }
        else
        {
            if (index < StrValue->size()-1)
            {
                this->PrintError("Index out of range");
                Variable var;
                return var;
            }
            StrValue->erase((StrValue->end() + index));
        }
    }
    //cout << (ListValue.begin() + index)->get_value() << endl;
    Variable returningVar;
    return returningVar;
}