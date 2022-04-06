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

void UpdateVars(string VariableNameCheck, string ComparationValue, Interpreter* interpreter, string* Value1, string* Value2)
{
    Variable VAR1;
    Variable VAR2;
    Variable* var1 = &VAR1;
    Variable* var2 = &VAR2;

    string value1 = VariableNameCheck;
    string value2 = ComparationValue;

    if (VariableNameCheck[0] != '"')
        var1 = interpreter->find_variable_pointer(VariableNameCheck);
    if (ComparationValue[0] != '"')
        var2 = interpreter->find_variable_pointer(ComparationValue);
    
    string Type1 = var1->get_type();
    if (Type1 != "")
    {
        if (Type1 == "int")
        {
            value1 = to_string(var1->get_int_value());
        }
        else if (Type1 == "string")
        {
            value1 = var1->get_str_value();
        }
    }

    *Value1 = value1;
    *Value2 = value2;
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
        "<",
        "=",
    };

    for (int index = 0; index < WhileCondition.size(); index++)
    {
        string String = WhileCondition[index];
        //cout << String << endl;
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

    //cout << VariableNameCheck << " " << Comparation << " " << ComparationValue << endl;

    string Value1;
    string Value2;

    UpdateVars(VariableNameCheck, ComparationValue, &interpreter, &Value1, &Value2);

    if (Comparation == "==")
    {
        //cout << Value1 << " " << Value2 << endl;
        while (Value1 == Value2)
        {
            //cout << Value1 << " " << Value2 << endl;
            //cout << "ok" << endl;
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
    else if (Comparation == ">")
    {
        try
        {
            int IntValue1 = stoi(Value1);
            int IntValue2 = stoi(Value2);
            while (Value1 > Value2)
            {
                //cout << Value1 << " " << Value2 << endl;
                //cout << "ok" << endl;
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
                IntValue1 = stoi(Value1);
                IntValue2 = stoi(Value2);
            }
        }
        catch(const std::exception& e)
        {
            cout << "Error: invalid type in while" << endl;
            return;
        }
    }
    else if (Comparation == "<")
    {
        try
        {
            int IntValue1 = stoi(Value1);
            int IntValue2 = stoi(Value2);
            while (Value1 < Value2)
            {
                //cout << Value1 << " " << Value2 << endl;
                //cout << "ok" << endl;
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
                IntValue1 = stoi(Value1);
                IntValue2 = stoi(Value2);
            }
        }
        catch(const std::exception& e)
        {
            cout << "Error: invalid type in while" << endl;
            return;
        }
    }
    else if (Comparation == "<=" || Comparation == "=<")
    {
        try
        {
            int IntValue1 = stoi(Value1);
            int IntValue2 = stoi(Value2);
            while (Value1 <= Value2)
            {
                //cout << Value1 << " " << Value2 << endl;
                //cout << "ok" << endl;
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
                IntValue1 = stoi(Value1);
                IntValue2 = stoi(Value2);
            }
        }
        catch(const std::exception& e)
        {
            cout << "Error: invalid type in while" << endl;
            return;
        }
    }
    else if (Comparation == ">=" || Comparation == "=>")
    {
        try
        {
            int IntValue1 = stoi(Value1);
            int IntValue2 = stoi(Value2);
            while (Value1 >= Value2)
            {
                //cout << Value1 << " " << Value2 << endl;
                //cout << "ok" << endl;
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
                IntValue1 = stoi(Value1);
                IntValue2 = stoi(Value2);
            }
        }
        catch(const std::exception& e)
        {
            cout << "Error: invalid type in while" << endl;
            return;
        }
    }
}

void While::add_condition(vector<string> ComparationLine)
{
    this->WhileCondition = ComparationLine;
}
