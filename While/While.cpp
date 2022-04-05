#include "../Other/Utilities/Utilities.h"
#include <iostream>
#include <vector>
#include <string>
#include "While.h"
#include "../Interpreter/Interpreter.h"

using namespace std;
using namespace Utilities;

void While::add_line(vector<string> line)
{
    //cout << line.size() << endl;
    // Ao er contributore
    this->lines.push_back(line);
}

void UpdateVars(string ComparationValue, string VariableNameCheck, Interpreter* interpreter, string* Value1, string* Value2)
{
    Variable* var1 = interpreter->find_variable_pointer(VariableNameCheck);
    Variable* var2 = interpreter->find_variable_pointer(ComparationValue);
    string Type1 = var1->get_type();

    string value1;
    string value2 = ComparationValue;
    if (Type1 == "int")
    {
        value1 = to_string(var1->get_int_value());
    }
    Value1 = &value1;
    Value2 = &value2;
}

void While::execute(std::vector<Variable> Variables)
{
    Interpreter interpreter(Variables, true);
    //Check
    string VariableNameCheck;
    string Comparation;
    string ComparationValue;

    vector<string> Comparations = {
            "==",
            ">",
            "<"
    };

    for (int index = 0; index < WhileCondition.size(); index++)
    {
        string String = WhileCondition[index];
        cout << String << endl;
        if (findInVector(Comparations, String))
        {
            //cout << "Simbolo" << endl;
            Comparation += String;
            int num = 1;
            if (!findInVector(Comparations, WhileCondition[index - 1]))
                VariableNameCheck = WhileCondition[index - 1];
            if (!findInVector(Comparations, WhileCondition[index + 1]))
                ComparationValue = WhileCondition[++index];
        }
    }

    cout << VariableNameCheck << " " << Comparation << " " << ComparationValue << endl;

    string Value1;
    string Value2;

    UpdateVars(VariableNameCheck, ComparationValue, &interpreter, &Value1, &Value2);

    if (Comparation == "==")
    {

        while (Value1 == Value2)
        {
            cout << "ok" << endl;
            vector<vector<string>> lines = this->lines;
            for (int i = 0; i < lines.size(); i++)
            {
                vector<string> line = lines[i];
                string StrLine;
                for (int j = 0; j < line.size(); j++) {
                    StrLine += (line[j] + " ");
                }
                interpreter.Line(StrLine);
            }
            UpdateVars(VariableNameCheck, ComparationValue, &interpreter, &Value1, &Value2);
        }
    }
}

void While::add_condition(vector<string> ComparationLine)
{
    this->WhileCondition = ComparationLine;
}
