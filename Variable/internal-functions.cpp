#include <string>
#include "Variable.h"
#include "../Interpreter/Interpreter.h"
#include "../Other/Utilities/Utilities.h"

using namespace std;
using namespace interpreter;
using namespace Utilities;

void Interpreter::internal_add(Variable* variable, vector<string> parameters)
{
    if (parameters.empty())
    {
        //*Error = true;
        this->PrintError("No parameters given");
        return;
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
}

void Interpreter::internal_remove(Variable* variable, vector<string> parameters)
{
    if (parameters.empty())
    {
        this->PrintError("No parameters given");
        return;
    }

    if (isNan(parameters[0]))
    {
        this->PrintError("Invalid index");
        return;
    }

    int index = stoi(parameters[0]);

    vector<Variable>* ListValue = variable->get_list_value_pointer();

    if (index >= 0)
    {
        if (index > ListValue->size()-1)
        {
            this->PrintError("Index out of range");
            return;
        }
        ListValue->erase((ListValue->begin() + index));
    }
    else
    {
        if (index < ListValue->size()-1)
        {
            this->PrintError("Index out of range");
            return;
        }
        ListValue->erase((ListValue->end() + index));
    }
    //cout << (ListValue.begin() + index)->get_value() << endl;
}