#include "../Other/Utilities/Utilities.h"
#include <iostream>
#include <vector>
#include <string>
#include "While.h"
#include "../Interpreter/Interpreter.h"

using namespace std;
using namespace Utilities;
using namespace interpreter;

void While::add_line(vector<string> line)
{
    //cout << line.size() << endl;
    // Ao er contributore
    vector<string> newline;
    int Opened = 0;
    int Closed = 0;
    for (int i = 0; i < line.size(); ++i) {
        string Word = line[i];
        if (Word == "{" || Word == "}")
        {
            if (Opened != 0 && Opened != Closed)
            {
                //cout << Word;
                newline.push_back(Word);
            }
        }
        else
        {
            //cout << Word;
            newline.push_back(Word);
        }
        if (Word == "{")
            Opened++;
        if (Word == "}")
            Closed++;
    }
    /*cout << "Printing everything" << endl;
    for (int i = 0; i < newline.size(); ++i)
    {
        string line = newline[i];
        cout << line << " ";
    }
    cout << endl;*/
    this->lines.push_back(newline);
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

bool Update(Interpreter* interpreter, While* aWhile)
{
    vector<vector<string>> conditions = {{}};

    vector<string> ValidOperators1 = {
            "&&",
            "||"
    };

    vector<string> operators;
    int conditionnum = 0;
    for (int j = 0; j < aWhile->WhileCondition.size(); ++j) {
        string param = aWhile->WhileCondition[j];
        if (findInVector(ValidOperators1, param))
        {
            operators.push_back(param);
            conditionnum++;
            conditions.push_back({});
        }
        else
        {
            vector<string>* Condition = &conditions[conditionnum];

            if (Condition->size() < 3)
            {
                Condition->push_back(param);
            }
        }
    }

    if (conditions.size() == 0)
    {
        interpreter->PrintError("No conditions");
        return false;
    }

    vector<bool> Checks = {};
    int num = 0;

    vector<string> ValidOperators = {
            "==",
            ">=",
            "=>",
            "<=",
            "=<",
            "<",
            ">"
    };

    for (int j = 0; j < conditions.size(); ++j) {
        vector<string> Condition = conditions[j];

        if (Condition.size() < 3) {
            interpreter->PrintError("Not enough conditions");
            return false;
        }

        if (!findInVector(ValidOperators, Condition[1])) {
            interpreter->PrintError("Invalid operator for condition");
            return false;
        }

        string if1 = Condition[0];
        string Comparator = Condition[1];
        string if2 = Condition[2];

        bool isStr1 = (if1[0] == '"');
        bool isStr2 = (if2[0] == '"');

        string Val1;
        string Val2;

        string Type1;
        string Type2;

        if (isStr1)
        {
            Val1 = if1;
            Type1 = "string";
        }
        else if (!isNan(if1))
        {
            Val1 = if1;
            Type1 = "int";
        }
        else if (if1 == "true" || if1 == "false")
        {
            Val2 = if2;
            Type2 = "bool";
        }
        else
        {
            Variable var1 = interpreter->find_variable(if1);

            if (var1.get_type() == "")
            {
                interpreter->PrintError("Variable not found");
                return false;
            }
            else
            {
                Type1 = var1.get_type();

                if (Type1 == "int")
                {
                    Val1 = to_string(var1.get_int_value());
                }
                else if (Type1 == "string")
                {
                    Val1 = var1.get_str_value();
                }
                else if (Type1 == "bool")
                {
                    bool VAL = var1.get_bool_value();
                    if (VAL)
                        Val1 = "true";
                    else
                        Val1 = "false";
                }
            }
        }

        if (isStr2)
        {
            Val2 = if2;
            Type2 = "string";
        }
        else if (!isNan(if2))
        {
            Val2 = if2;
            Type2 = "int";
        }
        else if (if2 == "true" || if2 == "false")
        {
            Val2 = if2;
            Type2 = "bool";
        }
        else
        {
            Variable var2 = interpreter->find_variable(if2);
            if (var2.get_type() == "")
            {
                interpreter->PrintError("Variable not found");
                return false;
            }
            Type2 = var2.get_type();
            if (Type2 == "int")
            {
                Val2 = to_string(var2.get_int_value());
            }
            else if (Type2 == "string")
            {
                Val2 = var2.get_str_value();
            }
            else if (Type2 == "bool")
            {
                bool VAL = var2.get_bool_value();
                if (VAL)
                    Val2 = "true";
                else
                    Val2 = "false";
            }
        }

        bool FinalValue;

        if (Comparator == "==") {
            if (Type1 == Type2)
            {
                if (Type1 == "int")
                {
                    if (stoi(Val1) == stoi(Val2)) {
                        //cout << "true" << endl;
                        FinalValue = true;
                    } else {
                        //cout << "false" << endl;
                        FinalValue = false;
                    }
                }
                else if (Type1 == "string")
                {
                    if (Val1 == Val2) {
                        //cout << "true" << endl;
                        FinalValue = true;
                    } else {
                        //cout << "false" << endl;
                        FinalValue = false;
                    }
                }
                else if (Type1 == "bool")
                {
                    if (to_bool(Val1) == to_bool(Val2)) {
                        //cout << "true" << endl;
                        FinalValue = true;
                    } else {
                        //cout << "false" << endl;
                        FinalValue = false;
                    }
                }
            }
            else
            {
                if (Val1 == Val2) {
                    //cout << "true" << endl;
                    FinalValue = true;
                } else {
                    //cout << "false" << endl;
                    FinalValue = false;
                }
            }
        }
        else if (Comparator == ">=" || Comparator == "=>")
        {
            if (Val1 >= Val2)
            {
                //cout << "true" << endl;
                FinalValue = true;
            }
            else
            {
                //cout << "false" << endl;
                FinalValue = false;
            }
        }
        else if (Comparator == "<=" || Comparator == "=<")
        {
            if (Val1 <= Val2)
            {
                //cout << "true" << endl;
                FinalValue = true;
            }
            else
            {
                //cout << "false" << endl;
                FinalValue = false;
            }
        }
        else if (Comparator == ">")
        {
            if (Val1 > Val2)
            {
                //cout << "true" << endl;
                FinalValue = true;
            }
            else
            {
                //cout << "false" << endl;
                FinalValue = false;
            }
        }
        else if (Comparator == "<")
        {
            if (Val1 < Val2)
            {
                //cout << "true" << endl;
                FinalValue = true;
            }
            else
            {
                //cout << "false" << endl;
                FinalValue = false;
            }
        }

        if (Checks.size() == 0)
            Checks.push_back(FinalValue);
        else
        {
            int index = Checks.size()-1;
            if (operators[num] == "&&")
                Checks[index] = ((FinalValue && Checks[index]));
            if (operators[num] == "||")
                Checks[index] = ((FinalValue || Checks[index]));
            num++;
        }
    }
    //cout << Checks[0] << endl;
    return Checks[0];
}

void While::execute(std::vector<Variable> Variables)
{
    Function func;
    Interpreter interpreter(Variables, true, &func);
    //Check
    /*string VariableNameCheck;
    string Comparation;
    string ComparationValue;

    vector<string> Comparations = {
        "==",
        ">",
        "<",
    };

    vector<vector<string>> conditions = {};

    for (int index = 0; index < WhileCondition.size(); index++)
    {
        string String = WhileCondition[index];
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
        cout << Value1 << " " << Value2 << endl;
        while (Value1 == Value2)
        {
            cout << Value1 << " " << Value2 << endl;
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
            interpreter.PrintError("Invalid type in while");
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
            interpreter.PrintError("Invalid type in while");
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
            interpreter.PrintError("Invalid type in while");
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
            interpreter.PrintError("Invalid type in while");
            return;
        }
    }*/

    bool Updated = Update(&interpreter, this);

    //cout << "Final = " << Updated << endl;
    while (Updated)
    {
        //cout << Value1 << " " << Value2 << endl;
        vector<vector<string>> lines = this->lines;
        for (int i = 0; i < lines.size(); i++)
        {
            vector<string> line = lines[i];
            string StrLine;
            for (int j = 0; j < line.size(); j++) {
                StrLine += (line[j] + " ");
            }
            //cout << StrLine << endl;
            interpreter.Line(StrLine);
        }
        Updated = Update(&interpreter, this);
        //interpreter.debugVariables();
        //cout << "--------------" << endl;
    }
}

void While::add_condition(vector<string> ComparationLine)
{
    /*for (int i = 0; i < ComparationLine.size(); ++i)
    {
        cout << "test = " << ComparationLine[i] << endl;
    }*/
    this->WhileCondition = ComparationLine;
}

void Interpreter::WhileLoop(const vector<string> condition)
{
    /*while (Value1 == Value2)
    {
        cout << "While" << endl;
    }*/
    this->writingWhile = true;
    While whileloop;
    whileloop.add_condition(condition);
    this->whiles.push_back(whileloop);
}