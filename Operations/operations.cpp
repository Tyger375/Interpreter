#include "../Interpreter/Interpreter.h"
#include "../Function/Function.h"
#include "../Other/Utilities/Utilities.h"
#include <iostream>

using namespace std;
using namespace Utilities;

bool interpreter::Interpreter::Addition(string* value, string* typeFinalValue, vector<string> splitted, string type1, int* j)
{
    const string add1 = *value;
    string add2 = splitted[((*j) += 2)];

    Variable var = this->find_variable(add2);

    string type = var.get_type();

    if (splitted.size() > (*j+1) && splitted[*j + 1] == "(")
    {
        string namefunction = splitted[i];

        *j += 2;

        string String = splitted[*j];
        vector<string> parameters;
        bool CheckWriting = writingFunc || writingWhile;
        int num = 0;
        do
        {
            //cout << splitted.size() << " " << *j << endl;
            if (*j >= splitted.size()) break;

            if (String == ")")
                break;

            if (String != ",")
            {
                if (String == "(")
                {
                    string FunctionName = splitted[*j-1];
                    vector<string> params;
                    *j++;
                    while (true)
                    {
                        string param = splitted[*j];
                        if (param == ")")
                        {
                            num--;
                            break;
                        }
                        if (param != ",")
                            params.push_back(param);
                        if (*j >= splitted.size()-1)
                            break;
                        *j++;
                    }

                    //executing function
                    Function FUNCTION = this->find_function(FunctionName);
                    if (FUNCTION.get_name() == "")
                    {
                        this->PrintError("Invalid function");
                        return false;
                    }
                    else
                    {
                        bool Returning;
                        Variable returned = this->executeFunction(FunctionName, CheckWriting, params, false, &Returning);
                        parameters[parameters.size()-1] = returned.get_value();
                    }
                }
                else if (num <= 0 && String == ")")
                {
                    if (num <= 0)
                        break;
                }
                else if (String == "=" && splitted[*j + 1] == "=")
                {
                    parameters.push_back("==");
                    *j++;
                }
                else
                    parameters.push_back(String);
            }

            if (*j < splitted.size()-1)
            {
                *j += 1;
                String = splitted[*j];
            }
            else
                break;
        } while (true);

        /*for (int j = 0; j < parameters.size(); ++j) {
            cout << "param = " << parameters[j] << endl;
        }*/

        bool Returning;
        Variable returnedVar = this->executeFunction(namefunction, CheckWriting, parameters, false, &Returning);
        string type2 = returnedVar.get_type();
        if (!Returning)
        {
            this->PrintError("Function returned nothing");
            return false;
        }

        if (type2 == "string")
            add2 = returnedVar.get_str_value();
        else if (type2 == "int")
            add2 = to_string(returnedVar.get_int_value());
        type = type2;
    }
    else if (type != "")
    {
        if (type == "string")
            add2 = var.get_str_value();
        else if (type == "int")
            add2 = to_string(var.get_int_value());
        else if (type == "bool")
            add2 = var.get_value();
    }
    else
        type = getTypeVar(add2);

    if (type1 == "")
        type1 = getTypeVar(add1);
    if (type == "")
    {
        this->PrintError("Invalid variable");
        return false;
    }

    if (type1 != type)
    {
        string newVal1;
        string newVal2;
        for (int i = 0; i < add1.length(); i++)
        {
            if (add1[i] != '"')
            {
                newVal1 += add1[i];
            }
        }
        for (int i = 0; i < add2.length(); i++)
        {
            if (add2[i] != '"')
            {
                newVal2 += add2[i];
            }
        }

        *value = (newVal1 + newVal2);
        *typeFinalValue = "string";
        return true;
    }
    else
    {
        *typeFinalValue = type1;
        if (type1 == "string" || type1 == "bool")
        {
            string result = add1 + add2;
            string newResult;
            for (int i = 0; i < result.size(); i++)
            {
                string character = string(1, result[i]);
                if (i == 0 || i == result.size() - 1)
                {
                    string charact = string(1, '"');
                    newResult += (character.c_str()[0] != '"') ? charact + character : charact;
                }
                else
                {
                    const char character = result[i];
                    if (character != '"')
                        newResult += character;
                }
            }
            *value = newResult;
            return true;
        }
        else if (type1 == "int")
        {
            int result = stoi(add1) + stoi(add2);
            *value = to_string(result);
            return true;
        }
    }
    return false;
}

bool interpreter::Interpreter::Subtraction(string* value, string* typeFinalValue, vector<string> splitted, string type1, int* j)
{
    const string add1 = *value;
    string add2 = splitted[(*j += 2)];

    Variable var = this->find_variable(add2);

    string type = var.get_type();

    if (splitted.size() > (*j+1) && splitted[*j + 1] == "(")
    {
        string namefunction = splitted[*j];

        *j += 2;

        string String = splitted[*j];
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
                if (String == "(")
                {
                    string FunctionName = splitted[*j-1];
                    vector<string> params;
                    *j++;
                    while (true)
                    {
                        string param = splitted[*j];
                        if (param == ")")
                        {
                            num--;
                            break;
                        }
                        if (param != ",")
                            params.push_back(param);
                        if (*j >= splitted.size()-1)
                            break;
                        *j++;
                    }

                    //executing function
                    Function FUNCTION = this->find_function(FunctionName);
                    if (FUNCTION.get_name() == "")
                    {
                        this->PrintError("Invalid function");
                        return false;
                    }
                    else
                    {
                        bool Returning;
                        Variable returned = this->executeFunction(FunctionName, CheckWriting, params, false, &Returning);
                        parameters[parameters.size()-1] = returned.get_value();
                    }
                }
                else if (num <= 0 && String == ")")
                {
                    if (num <= 0)
                        break;
                }
                else if (String == "=" && splitted[*j + 1] == "=")
                {
                    parameters.push_back("==");
                    *j++;
                }
                else
                    parameters.push_back(String);
            }

            if (*j < splitted.size()-1)
                *j++;
            else
                break;
            String = splitted[*j];
        } while (true);

        /*for (int j = 0; j < parameters.size(); ++j) {
            cout << "param = " << parameters[j] << endl;
        }*/

        bool Returning;
        Variable returnedVar = this->executeFunction(namefunction, CheckWriting, parameters, false, &Returning);
        string type2 = returnedVar.get_type();
        if (!Returning)
        {
            this->PrintError("Function returned nothing");
            return false;
        }

        if (type2 == "string")
            add2 = returnedVar.get_str_value();
        else if (type2 == "int")
            add2 = to_string(returnedVar.get_int_value());
        type = type2;
    }
    else if (type != "")
    {
        if (type == "string")
            add2 = var.get_str_value();
        else if (type == "int")
            add2 = to_string(var.get_int_value());
    }
    else
    {
        type = getTypeVar(add2);
    }
    if (type1 == "")
        type1 = getTypeVar(add1);
    if (type == "")
    {
        this->PrintError("Invalid variable");
        return false;
    }

    if (type1 != type)
    {
        /*string newVal1;
        string newVal2;
        for (int i = 0; i < add1.length(); i++)
        {
            if (add1[i] != '"')
            {
                newVal1 += add1[i];
            }
        }
        for (int i = 0; i < add2.length(); i++)
        {
            if (add2[i] != '"')
            {
                newVal2 += add2[i];
            }
        }

        *value = (newVal1 + newVal2);
        *typeFinalValue = "string";*/
        this->PrintError("Invalid type for - operation");
    }
    else
    {
        *typeFinalValue = type1;
        if (type1 == "string")
        {
            /*string result = add1 + add2;
            string newResult;
            for (int i = 0; i < result.size(); i++)
            {
                string character = string(1, result[i]);
                if (i == 0 || i == result.size() - 1)
                {
                    string charact = string(1, '"');
                    newResult += (character.c_str()[0] != '"') ? charact + character : charact;
                }
                else
                {
                    const char character = result[i];
                    if (character != '"')
                        newResult += character;
                }
            }
            //cout << result << " " << newResult << endl;
            *value = newResult;*/
            this->PrintError("Invalid type for - operation");
        }
        else if (type1 == "int")
        {
            int result = stoi(add1) - stoi(add2);
            *value = to_string(result);
        }
    }
    return true;
}