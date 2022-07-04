#include <string>
#include "Variable.h"
#include "../Interpreter/Interpreter.h"
#include "../Other/Utilities/Utilities.h"

using namespace std;
using namespace interpreter;
using namespace Utilities;

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
    //cout << (ListValue.begin() + index)->get_value() << endl;
    Variable returningVar;
    return returningVar;
}

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