#include "../Other/Utilities/Utilities.h"
#include <utility>
#include <vector>
#include <string>
#include "While.h"
#include "../Interpreter/Interpreter.h"

using namespace std;
using namespace Utilities;
using namespace interpreter;

void While::add_line(const vector<string>& line)
{
    vector<string> newline;
    for (const auto& Word : line) {
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

bool Update(Interpreter* interpreter, While* aWhile)
{
    vector<vector<string>> conditions = {{}};

    vector<string> ValidOperators1 = {
            "&&",
            "||"
    };

    vector<string> operators;
    int condition_num = 0;
    for (const auto& param : aWhile->WhileCondition)
    {
        if (findInVector(ValidOperators1, param))
        {
            operators.push_back(param);
            condition_num++;
            conditions.emplace_back();
        }
        else
        {
            vector<string>* Condition = &conditions[condition_num];

            if (Condition->size() < 3)
            {
                Condition->push_back(param);
            }
        }
    }

    if (conditions.empty())
    {
        interpreter->PrintError("No conditions");
        return false;
    }

    vector<bool> Checks = {};
    int num = 0;

    vector<string> ValidOperators = {
            "==",
            "!=",
            ">=",
            "=>",
            "<=",
            "=<",
            "<",
            ">"
    };

    for (auto Condition : conditions) {
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

        //cout << if1 << " " << if2 << endl;

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
            Val1 = if1;
            Type1 = "bool";
        }
        else
        {
            Variable var1 = interpreter->find_variable(if1);

            if (var1.get_type().empty())
            {
                interpreter->PrintError("Variable not found: " + if1);
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
            if (var2.get_type().empty())
            {
                interpreter->PrintError("Variable not found: " + if2);
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

        //cout << Val1 << " " << Val2 << endl;

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
                    //cout << Val1 << " " << Val2 << endl;
                    //cout << to_bool(Val1) << " " << to_bool(Val2) << endl;
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
        else if (Comparator == "!=") {
            if (Type1 == Type2)
            {
                if (Type1 == "int")
                {
                    if (stoi(Val1) != stoi(Val2)) {
                        //cout << "true" << endl;
                        FinalValue = true;
                    } else {
                        //cout << "false" << endl;
                        FinalValue = false;
                    }
                }
                else if (Type1 == "string")
                {
                    if (Val1 != Val2) {
                        //cout << "true" << endl;
                        FinalValue = true;
                    } else {
                        //cout << "false" << endl;
                        FinalValue = false;
                    }
                }
                else if (Type1 == "bool")
                {
                    if (to_bool(Val1) != to_bool(Val2)) {
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
                if (Val1 != Val2) {
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
            //cout << Val1 << " < " << Val2 << endl;
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

        //cout << FinalValue << endl;

        if (Checks.empty())
            Checks.push_back(FinalValue);
        else
        {
            unsigned int index = Checks.size()-1;
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

void While::execute(const std::vector<Variable>& Variables)
{
    /*cout << "start" << endl;
    for (auto line : this->lines)
    {
        for (auto word : line)
        {
            cout << word << " ";
        }
        cout << endl;
    }
    cout << "end" << endl;*/
    /*for (auto var : Variables)
    {
        cout << var.get_name() << " " << var.get_value() << endl;
    }*/
    
    Function func;
    Interpreter interpreter(Variables, true, &func);
    //interpreter.debugVariables();
    //Check
    bool Updated = Update(&interpreter, this);

    //cout << Updated << endl;

    while (Updated)
    {
        //cout << Value1 << " " << Value2 << endl;
        vector<vector<string>> local_lines = this->lines;
        for (const auto& line : local_lines)
        {
            string StrLine;
            for (auto & j : line) {
                StrLine += (j + " ");
            }
            //cout << "StrLine = " << StrLine << endl;
            interpreter.Line(StrLine);
            interpreter.line++;
        }
        Updated = Update(&interpreter, this);
        interpreter.line = 0;
        interpreter.clear();
        //interpreter.debugVariables();
    }
}

void Interpreter::clear()
{
    this->whiles.clear();
    this->writingWhile.clear();
}

void While::add_condition(vector<string> Comparison_Line)
{
    this->WhileCondition = move(Comparison_Line);
}

void Interpreter::WhileLoop(const vector<string>& condition)
{
    /*while (Value1 == Value2)
    {
        cout << "While" << endl;
    }*/
    this->writingWhile.push_back(true);
    //cout << writingWhile.size() << " " << this->line << endl;
    if (this->whiles.empty())
    {
        While while_loop;
        while_loop.add_condition(condition);
        this->whiles.push_back(while_loop);
    }
}