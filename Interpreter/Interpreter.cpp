#include "Interpreter.h"
#include "../Other/Utilities/Utilities.h"
#include <iostream>
#include <utility>

using namespace std;
using namespace Utilities;
using namespace interpreter;

void Interpreter::Setup()
{
	this->typeVariables = {
		"var"
	};
    this->line = 0;
    this->FindingElse = false;
	this->writingFunc = false;
    this->writingWhile = false;
    this->writingComment = false;
    //this->writingList = false;
    this->isExecutingFunc = false;
    this->error = false;

    vector<string> params;
    Function outFunc;
    outFunc.setup("out", params);

    Function whileFunc;
    whileFunc.setup("while", params);

    Function ifFunc;
    ifFunc.setup("if", params);

    Function inFunc;
    inFunc.setup("in", params);

    Function typeFunc;
    typeFunc.setup("type", params);

    this->functions.push_back(outFunc);
    this->functions.push_back(whileFunc);
    this->functions.push_back(ifFunc);
    this->functions.push_back(inFunc);
    this->functions.push_back(typeFunc);
}

Interpreter::Interpreter()
{
	this->Setup();
}

Interpreter::Interpreter(const vector<Variable>& oldVariables, bool executingFunction, Function* func)
{
	this->Setup();
    this->isExecutingFunc = executingFunction;
    this->FUNC = func;
	for (const auto & oldVariable : oldVariables)
	{
		this->variables.push_back(oldVariable);
	}
	
}

void Interpreter::start(const std::string& file_name, bool debug)
{
	fstream new_file;
	new_file.open(file_name, ios::in);
	if (new_file.is_open())
	{
		string tp;
		while (getline(new_file, tp))
		{
            if (this->error)
                break;
            this->line++;
			this->Line(tp);
		}
        if (debug)
        {
            this->debugVariables();
            this->debugFunctions();
        }

		/*for (int i = 0; i < this->VariablesInfos.size(); i++)
		{
			for (int i2 = 0; i2 < VariablesInfos[i].size(); i2++)
			{
				cout << "Variable in if " << i << " = " << VariablesInfos[i][i2].get_type() << endl;
			}
		}*/
		//cout << functions.size() << " " << functions[0].get_lines().size() << " " << functions[0].get_lines()[0][0] << endl;
		//cout << this->functions.size() << endl;
		/*for (int i = 0; i < this->functions.size(); i++)
		{
			//cout << "i = " << i << endl;
			cout << "function name: " << functions[i].get_name() << endl;
			for (int i2 = 0; i2 < this->functions[i].get_lines().size(); i2++)
			{
				//cout << "i2 = " << i2 << endl;
				vector<string> line = functions[i].get_lines()[i2];
				for (int i3 = 0; i3 < line.size(); i3++)
				{
					//cout << "i3 = " << i3 << endl;
					cout << line[i3] << " ";
				}
				cout << endl;
				//cout << "Variable in if " << i << " = " << VariablesInfos[i][i2].get_type() << endl;
			}
		}*/
		new_file.close();
	}
	else
	{
		this->PrintError("File not found");
	}
}

void Interpreter::Line(string str_line)
{
	try
	{
		vector<string> splitted = split(move(str_line), ' ');
		/*for (int j = 0; j < splitted.size(); j++)
		{
			cout << splitted[j] << endl;
		}*/
		string lastString;
		for (this->i = 0; this->i < splitted.size(); this->i++)
		{
            if (this->error) break;
			const string String = splitted[i];

            //cout << String << endl;

            //cout << Ifs.size() << " " << this->ListWriting.size() << " " << this->writingFunc << " " << this->writingWhile << endl;

            if (this->i == 0 && String != "else")
            {
                if (this->FindingElse && this->line != this->FindingFromLine)
                {
                    this->FindingElse = false;
                    this->Ifs.erase(this->Ifs.end() - 1);
                }
            }

			if (String == "/")
			{
                if (i > splitted.size()-1)
                    return;
				if (splitted[i + 1] == "/")
				{
                    i++;
					return;
				}
                if (splitted[i+1] == "*")
                {
                    i++;
                    writingComment = true;
                }
			}

            if (String == "*" && splitted[i+1] == "/")
            {
                i++;
                writingComment = false;
                if (i > splitted.size()-1)
                    return;
            }

			if (this->Ifs.empty())
			{
                if (i == (splitted.size()-1))
                {
                    if (!this->functions.empty() && writingFunc)
                        this->functions[functions.size() - 1].add_line(splitted);
                    else if (!this->whiles.empty() && writingWhile)
                        this->whiles[whiles.size() - 1].add_line(splitted);
                }
			}

            if (!this->Ifs.empty())
            {
                if (!this->Ifs[Ifs.size()-1] && !this->FindingElse)
                {
                    if (String == "}")
                    {
                        //this->Ifs.erase(this->Ifs.end() - 1);
                        this->FindingElse = true;
                        this->FindingFromLine = this->line;
                    }
                    else
                        return;
                }
            }
            if (!this->writingList.empty() && this->writingList[writingList.size()-1])
                this->loadList(splitted, true, &i);

			bool FoundInVector = findInVector(typeVariables, lastString);
            bool CheckWriting = writingFunc || writingWhile || writingComment;

			if (!findInVector(typeVariables, String) && this->writingList.empty())
			{
				if (!FoundInVector)
				{
					if (String == "=" && !CheckWriting)
					{
						//const
						string name = lastString;
                        if (lastString == "]")
                        {
                            //accessing member
                            bool found = false;
                            int index = this->i;
                            vector<string> indexes;
                            do
                            {
                                if (lastString == "]")
                                {
                                    index--;
                                    if (isNan(splitted[index-1]))
                                    {
                                        this->PrintError("Invalid index");
                                    }
                                    else
                                    {
                                        index--;
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
                                                indexes.push_back(splitted[index+1]);
                                                if (splitted[index-1] != "]")
                                                    found = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (found)
                                {
                                    name = splitted[index-1];
                                    break;
                                }
                            }
                            while (!found);

                            Variable VAR = find_variable(name);
                            string ListValue = GetListValue(VAR);

                            vector<string> Splitted = split(ListValue, ' ');

                            int index2 = 0;
                            //this->writingList.push_back(true);
                            this->loadList(Splitted, false, &index2);

                            Variable OriginList = this->ListWriting[0];
                            Variable* List = &OriginList;
                            Variable ListToWrite = OriginList;

                            //this->ListWriting.erase(ListWriting.end()-1);

                            i++;

                            vector<string> new_splitted;

                            for (; i < splitted.size(); ++i)
                            {
                                new_splitted.push_back(splitted[i]);
                            }

                            Variable NewValue = this->loadVariableWithoutWriting(new_splitted, "");

                            for (unsigned int j = indexes.size()-1; j >= 0; --j)
                            {
                                //member inside a list
                                string Index = indexes[j];
                                if (Index == "]")
                                {
                                    this->PrintError("No index");
                                    return;
                                }
                                if (getTypeVar(Index) != "int")
                                {
                                    this->PrintError("Invalid index type");
                                    return;
                                }
                                index++;

                                if (List->get_list_value().size() <= stoi(Index))
                                {
                                    this->PrintError("Index out of range");
                                    return;
                                }
                                Variable member = List->get_list_value()[stoi(Index)];
                                string type = member.get_type();
                                if (List->get_type() == "list")
                                {
                                    vector<Variable> Vector = List->get_list_value();
                                    Variable* Pointer = List->get_item_list_pointer(stoi(Index));
                                    List = Pointer;
                                }
                                else
                                {
                                    this->PrintError("Invalid syntax");
                                    return;
                                }
                                if (j == 0)
                                    *List = NewValue;
                            }
                            while (!this->ListWriting.empty())
                            {
                                this->ListWriting.erase(ListWriting.end()-1);
                            }
                            while (!this->writingList.empty())
                            {
                                this->writingList.erase(writingList.end()-1);
                            }

                            bool Found = false;
                            Variable newVar;
                            newVar.setup(name, OriginList.get_list_value());
                            *this->find_variable_pointer(name) = newVar;
                        }
                        else
                            this->loadVariable(splitted, name);
					}
                    else if (String == "." && !CheckWriting)
                    {
                        Variable* VARIABLE;

                        string name = lastString;
                        Variable OriginList;
                        Variable *List = &OriginList;

                        bool with_index = false;

                        if (lastString == "]")
                        {
                            with_index = true;
                            //accessing member
                            bool found = false;
                            int index = this->i;
                            vector<string> indexes;
                            do
                            {
                                if (lastString == "]")
                                {
                                    index--;
                                    if (isNan(splitted[index - 1]))
                                    {
                                        this->PrintError("Invalid index");
                                    }
                                    else
                                    {
                                        index--;
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
                                    name = splitted[index - 1];
                                    break;
                                }
                            } while (!found);

                            Variable VAR = find_variable(name);
                            string ListValue = GetListValue(VAR);

                            vector<string> Splitted = split(ListValue, ' ');

                            int index2 = 0;
                            //this->writingList.push_back(true);
                            this->loadList(Splitted, false, &index2);

                            OriginList = this->ListWriting[0];
                            Variable ListToWrite = OriginList;

                            //this->ListWriting.erase(ListWriting.end()-1);

                            i++;

                            for (unsigned int j = indexes.size() - 1; j >= 0; --j)
                            {
                                //member inside a list
                                string Index = indexes[j];
                                if (Index == "]")
                                {
                                    this->PrintError("No index");
                                    return;
                                }
                                if (getTypeVar(Index) != "int")
                                {
                                    this->PrintError("Invalid index type");
                                    return;
                                }
                                index++;

                                if (List->get_list_value().size() <= stoi(Index))
                                {
                                    this->PrintError("Index out of range");
                                    return;
                                }
                                Variable member = List->get_list_value()[stoi(Index)];
                                string type = member.get_type();
                                {
                                    if (type == "list")
                                    {
                                        vector<Variable> Vector = List->get_list_value();
                                        Variable *Pointer = List->get_item_list_pointer(stoi(Index));
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
                            i--;
                        }
                        else
                            VARIABLE = this->find_variable_pointer(name);

                        if (i-1 >= 0)
                        {
                            if (i < splitted.size()-1)
                            {
                                string function_name = splitted[i + 1];
                                if (i+1 < splitted.size()-1 && splitted[i+2] == "(")
                                {
                                    vector<string> parameters;
                                    this->i += 3;
                                    this->WriteParameters(splitted, &parameters, CheckWriting, false, &this->i);
                                    this->execute_internal_function(VARIABLE, function_name, parameters);
                                    /*cout << "Origin list = ";
                                    this->printList(OriginList);
                                    cout << endl;*/
                                    if (with_index)
                                    {
                                        Variable newVar;
                                        newVar.setup(name, OriginList.get_list_value());
                                        *this->find_variable_pointer(name) = newVar;
                                    }
                                    if (error)
                                    {
                                        //this->PrintError("Error");
                                        return;
                                    }
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
					else if (String == "+" && !CheckWriting)
					{
						string add1 = lastString;
						string add2 = splitted[(++this->i)];

						Variable Var = this->find_variable(add1);
						const string type = Var.get_type();
						if (!type.empty())
						{
							if (type == "string")
								add1 = Var.get_str_value();
							else if (type == "int")
								add1 = to_string(Var.get_int_value());
						}

						Variable Var2 = this->find_variable(add2);
						const string type2 = Var2.get_type();
						if (!type2.empty())
						{
							if (type2 == "string")
								add2 = Var2.get_str_value();
							else if (type2 == "int")
								add2 = to_string(Var2.get_int_value());
						}
						bool isNan1 = isNan(add1);
						bool isNan2 = isNan(add2);

						if (isNan1 || isNan2)
						{
							string value = add1 + add2;
							string typeFinalValue;
							this->AddStrings(splitted, &value, &typeFinalValue);
						}
						else
						{
							string value = to_string(stoi(add1) + stoi(add2));
							string typeFinalValue;
							this->AddIntegers(splitted, &value, &typeFinalValue);
						}
					}
                    else if (String == "return" && this->isExecutingFunc)
                    {
                        this->SetReturnValue(splitted);
                    }
                    else if (String == "else" && this->FindingElse)
                    {
                        this->FindingElse = false;
                        this->Ifs[Ifs.size()-1] = !Ifs[Ifs.size()-1];
                    }
					else if (String == "(")
					{
						if (lastString.empty())
                        {
                            this->PrintError("Not a function");
                            return;
                        }
                        const string& name_function = lastString;
                        bool IsNewFunc = false;

                        if ((i - 2) >= 0)
                            if(splitted[i-2] == "func")
                                IsNewFunc = true;

                        vector<string> parameters;

                        int num = 0;

                        this->i++;

                        string string_check = splitted[this->i];

                        if (CheckWriting)
                        {
                            int Integer = this->i;
                            this->WriteParameters(splitted, &parameters, CheckWriting, IsNewFunc, &Integer);
                        }
                        else
                            this->WriteParameters(splitted, &parameters, CheckWriting, IsNewFunc, &this->i);

                        bool Returning;
                        this->executeFunction(name_function, CheckWriting, parameters, IsNewFunc, &Returning);
					}
					else if (String == "{")
					{
						if (!this->Ifs.empty())
						{
							const unsigned int last_elem = Ifs.size() - 1;
							if (!this->Ifs[last_elem])
							{
								//If is false
							}
						}
						else
						{
							if (!this->functions.empty())
							{
								writingFunc = true;
								//cout << "Function opened" << endl;
							}
							else
							{
								//cout << "Procedures opened" << endl;
							}
						}
					}
					else if (String == "}")
					{
						if (!this->Ifs.empty())
						{
                            this->FindingElse = true;
                            this->FindingFromLine = this->line;
							if (!VariablesInfos.empty())
								this->VariablesInfos.erase(VariablesInfos.end() - 1);

							if (writingFunc && i == (splitted.size()-1))
								this->functions[functions.size() - 1].add_line(splitted);
                            if (writingWhile && i == (splitted.size()-1))
                                this->whiles[whiles.size() - 1].add_line(splitted);
						}
						else
						{
							if (!this->functions.empty() && writingFunc)
							{
								writingFunc = false;
							}
                            else if (!this->whiles.empty())
                            {
                                writingWhile = false;
                                this->whiles[whiles.size()-1].execute(this->variables);
                            }
							else
							{
								//cout << "Procedures closed" << endl;
							}
						}
					}
				}
			}

            if (i == splitted.size())
                i = int(splitted.size() - 1);

            lastString = splitted[i];

			if (i == (splitted.size()-1))
			{
                if (writingFunc)
                {
                    if((String == "{" || String == "}" || String == ")") && (!this->Ifs.empty()))
                    {
                        this->functions[functions.size() - 1].add_line(splitted);
                    }
                }
                else if (writingWhile)
                {
                    if((String == "{" || String == "}" || String == ")") && (!this->Ifs.empty()))
                    {
                        this->whiles[whiles.size() - 1].add_line(splitted);
                    }
                }
			}
		}
        if (this->FindingElse && this->line != this->FindingFromLine)
        {
            this->FindingElse = false;
            this->Ifs.erase(this->Ifs.end() - 1);
        }
	}
	catch (const exception& error)
	{
		cout << "internal error: " << error.what() << endl;
	}
}

void Interpreter::Operation(std::vector<std::string> splitted, string* value, string* typeFinalValue, const string& type1, int* j)
{
	do
	{
		if ((*j + 1) >= splitted.size() || splitted[*j + 1].empty())
			break;

		if (splitted[*j + 1] == "+")
		{
            bool notError = this->Addition(value, typeFinalValue, splitted, type1, j);
            if (notError)
                break;
		}
        else if (splitted[*j + 1] == "-")
        {
            bool notError = this->Subtraction(value, typeFinalValue, splitted, type1, j);
            if (notError)
                break;
        }
	} while (true);
}

void Interpreter::AddIntegers(std::vector<std::string> splitted, string* value, string* typeFinalValue)
{
	do
	{
		if ((this->i + 1) >= splitted.size() || splitted[this->i + 1].empty())
			break;
		if (splitted[this->i + 1] == "+")
		{
			const string add1 = *value;
			string add2 = splitted[(this->i += 2)];

			Variable var = this->find_variable(add2);
			const string type = var.get_type();
			if (!type.empty())
			{
				if (type == "string")
					add2 = var.get_str_value();
				else if (type == "int")
					add2 = to_string(var.get_int_value());
			}

			bool isNan1 = isNan(add1);
			bool isNan2 = isNan(add2);

			if (isNan1 || isNan2)
			{
				this->PrintError("Invalid type of value");
				*typeFinalValue = "invalid";
				break;
			}
			else
			{
				int result = stoi(add1) + stoi(add2);

				*value = to_string(result);
				*typeFinalValue = "int";
			}
		}
	} while (true);
}

void Interpreter::AddStrings(std::vector<std::string> splitted, string* value, string* typeFinalValue)
{
	do
	{
		if ((this->i + 1) >= splitted.size() || splitted[this->i + 1].empty())
			break;
		if (splitted[this->i + 1] == "+")
		{
			const string add1 = *value;
			const string add2 = splitted[(this->i += 2)];
			string result = add1 + add2;
			string newResult;
			for (int j = 0; j < result.size(); j++)
			{
				if (j == 0 || j == result.size() - 1)
				{
					newResult += '"';
				}
				else
				{
					const char character = result[j];
					if (character != '"')
						newResult += character;
				}
			}
			*value = newResult;
			*typeFinalValue = "string";
		}
	} while (true);
}

void Interpreter::If(const vector<string>& parameters)
{
    vector<vector<string>> conditions = {{}};

    vector<string> ValidOperators1 = {
            "&&",
            "||"
    };

    vector<string> operators;
    int condition_num = 0;
    for (const auto& param : parameters) {
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
        this->PrintError("No conditions");
        this->Ifs.push_back(false);
        return;
    }

    vector<bool> Checks = {};
    int num = 0;

    vector<string> ValidOperators = {
            "==",
            "!=",
            ">=",
            "=>",
            "<=",
            "=<"
    };

    for (auto Condition : conditions) {
        if (Condition.size() < 3)
        {
            this->PrintError("Not enough conditions");
            this->Ifs.push_back(false);
            return;
        }

        if (!findInVector(ValidOperators, Condition[1]))
        {
            this->PrintError("Invalid operator for condition");
            this->Ifs.push_back(false);
            return;
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
            Val1 = if1;
            Type1 = "bool";
        }
        else
        {
            Variable var1 = this->find_variable(if1);

            if (var1.get_type().empty())
            {
                this->PrintError("Variable not found");
                this->Ifs.push_back(false);
                return;
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
            Variable var2 = this->find_variable(if2);
            if (var2.get_type().empty())
            {
                this->PrintError("Variable not found");
                this->Ifs.push_back(false);
                return;
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
                    if (stoi(Val1) == stoi(Val2))
                        FinalValue = true;
                    else
                        FinalValue = false;
                }
                else if (Type1 == "string")
                {
                    if (Val1 == Val2)
                        FinalValue = true;
                    else
                        FinalValue = false;
                }
                else if (Type1 == "bool")
                {
                    if (to_bool(Val1) == to_bool(Val2))
                        FinalValue = true;
                    else
                        FinalValue = false;
                }
            }
            else
            {
                if (Val1 == Val2)
                    FinalValue = true;
                else
                    FinalValue = false;
            }
        }
        else if (Comparator == "!=") {
            if (Type1 == Type2)
            {
                if (Type1 == "int")
                {
                    if (stoi(Val1) != stoi(Val2))
                        FinalValue = true;
                    else
                        FinalValue = false;
                }
                else if (Type1 == "string")
                {
                    if (Val1 != Val2)
                        FinalValue = true;
                    else
                        FinalValue = false;
                }
                else if (Type1 == "bool")
                {
                    if (to_bool(Val1) != to_bool(Val2))
                        FinalValue = true;
                    else
                        FinalValue = false;
                }
            }
            else
            {
                if (Val1 != Val2)
                    FinalValue = true;
                else
                    FinalValue = false;
            }
        }
        else if (Comparator == ">=" || Comparator == "=>")
        {
            if (Type1 == Type2)
            {
                if (Type1 == "int")
                {
                    if (stoi(Val1) >= stoi(Val2))
                        FinalValue = true;
                    else
                        FinalValue = false;
                }
                else if (Type1 == "string")
                {
                    if (Val1 >= Val2)
                        FinalValue = true;
                    else
                        FinalValue = false;
                }
            }
            else
            {
                if (Val1 >= Val2)
                    FinalValue = true;
                else
                    FinalValue = false;
            }
        }

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

    this->Ifs.push_back(Checks[0]);
}

void Interpreter::ForLoop(const vector<string>& assign, const vector<string>& check, const vector<string>& advancing)
{
	string type;
	string variableName;
	string assignment;
	string value;
	for (int j = 0; j < assign.size(); j++)
	{
		const string& String = assign[j];
		if (findInVector(this->typeVariables, String))
		{
			type = String;
		}
		else if (String == "=")
		{
			assignment = String;
			variableName = assign[j - 1];
		}
		if (!type.empty() && !variableName.empty() && !assignment.empty() && String != "=")
		{
			value += (String + " ");
		}
	}
	cout << type << " " << variableName << " " << assignment << " " << value << endl;

	//Check
	string VariableNameCheck;
	string Comparison;
	string ComparisonValue;

	vector<string> Comparisons = {
		"=",
		">",
		"<"
	};

	for (int index = 0; index < check.size(); index++)
	{
		const string& String = check[index];
		if (findInVector(Comparisons, String))
		{
			Comparison += String;
			int num = 1;
			if (!findInVector(Comparisons, check[index - 1]))
				VariableNameCheck = check[index-1];
			if (!findInVector(Comparisons, check[index + 1]))
                ComparisonValue = check[++index];
		}
	}

	cout << VariableNameCheck << " " << Comparison << " " << ComparisonValue << endl;

	string VarName;
	string Operator;

	vector<string> Operators = {
		"+",
		"-"
	};

	for (int index = 0; index < advancing.size(); index++)
	{
		const string& String = advancing[index];
		if (findInVector(Operators, String))
		{
			Operator += String;
			int num = 1;
			if (!findInVector(Operators, advancing[index-1]))
				VarName = advancing[index-1];
		}
	}

	cout << VarName << " " << Operator << endl;

	Variable varDeclared = this->find_variable(variableName);
	Variable varCondition1 = this->find_variable(VariableNameCheck);
	
	if (Comparison == "<")
	{
		string typeVarDeclared = varDeclared.get_type();
		string typeVarCondition1 = varCondition1.get_type();
		if (typeVarDeclared == "int")
		{

			for (int j = 0; j < 10; j++)
			{
				cout << "ok" << endl;
			}
		}
	}
}

void Interpreter::LoadParamVariable(vector<string>* params, bool IsNewFunc, vector<string>* parameters)
{
    vector<string> comparators = {
            "==",
            "!=",
            ">=",
            "=>",
            "<=",
            "=<",
            "<",
            ">"
    };
    if (!params->empty() && !IsNewFunc)
    {
        if (!findInVector(comparators, (*params)[0]))
        {
            Variable VAR = this->loadVariableWithoutWriting(*params, "");
            parameters->push_back(VAR.get_value());
        }
        else
        {
            for (const auto & param : *params)
            {
                parameters->push_back(param);
            }
        }
        params->clear();
    }
}

void Interpreter::WriteParameters(vector<string> splitted, vector<string>* parameters, bool CheckWriting, bool IsNewFunc, int* index)
{
    string string_check = splitted[*index];
    vector<string> params;
    vector<string> comparators = {
            "==",
            "!=",
            ">=",
            "=>",
            "<=",
            "=<",
            "<",
            ">"
    };
    int num = 0;
    do
    {
        bool IsComparator = true;
        string Comparator;
        if (*index >= splitted.size()) break;
        if (string_check == ",")
        {
            IsComparator = false;
            LoadParamVariable(&params, IsNewFunc, parameters);
        }
        else if (string_check == "(")
        {
            IsComparator = false;
            string FunctionName = splitted[(*index)-1];
            vector<string> params2;
            (*index)++;
            this->WriteParameters(splitted, &params2, CheckWriting, IsNewFunc, index);
            //executing function
            Function FUNCTION = this->find_function(FunctionName);
            if (FUNCTION.get_name().empty())
            {
                this->PrintError("Invalid function");
                return;
            }
            else
            {
                bool Returning;
                Variable returned = this->executeFunction(FunctionName, CheckWriting, params2, IsNewFunc, &Returning);
                //((*parameters)[parameters->size()-1]) = returned.get_value();
                params[params.size()-1] = returned.get_value();
            }
            //num++;
        }
        else if (string_check == ")")
        {
            IsComparator = false;
            if (num <= 0)
                break;
        }
        else if (string_check == "[")
        {
            IsComparator = false;
            //Array member
            string VariableName = splitted[(*index)-1];
            Variable var = this->find_variable(VariableName);
            bool isIndex = false;
            if (!var.get_type().empty())
            {
                if ((*index)+1 < splitted.size())
                {
                    string StrIndex = splitted[(*index)+1];
                    if (StrIndex == "]")
                    {
                        this->PrintError("No index");
                        return;
                    }
                    if (getTypeVar(StrIndex) != "int")
                    {
                        this->PrintError("Invalid index type");
                        return;
                    }
                    (*index)++;
                    isIndex = true;
                    params.erase(params.end() - 1);
                    Variable member = var.get_list_value()[stoi(StrIndex)];
                    string type = member.get_type();
                    if (type == "string")
                        params.push_back(member.get_str_value());
                    else if (type == "int")
                        params.push_back(to_string(member.get_int_value()));
                    else if (type == "bool")
                        params.push_back(to_string(member.get_bool_value()));
                    else if (type == "list")
                        params.push_back(GetListValue(member));
                }
            }
            else if (!parameters->empty() && getTypeVar((*parameters)[parameters->size()-1]) == "list")
            {
                //member inside a list
                string local_index = splitted[i + 1];
                if (local_index == "]")
                {
                    this->PrintError("No local_index");
                    return;
                }
                if (getTypeVar(local_index) != "int")
                {
                    this->PrintError("Invalid local_index type");
                    return;
                }
                i++;
                isIndex = true;
                int index2 = 0;
                //this->writingList.push_back(true);
                //this->loadList(split((*parameters)[parameters->size()-1], ' '), false, &index2);
                this->loadList(split(params[params.size()-1], ' '), false, &index2);
                Variable member = this->ListWriting[0].get_list_value()[stoi(local_index)];
                string type = member.get_type();
                params.erase(params.end()-1);
                if (type == "string")
                    params.push_back(member.get_str_value());
                else if (type == "int")
                    params.push_back(to_string(member.get_int_value()));
                else if (type == "bool")
                    params.push_back(to_string(member.get_bool_value()));
                else if (type == "list")
                    params.push_back(GetListValue(member));
                this->ListWriting.erase(ListWriting.end()-1);
                while (!this->writingList.empty())
                {
                    this->writingList.erase(writingList.end()-1);
                }
            }
            if (!isIndex)
            {
                string FinalString = "[";
                int num2 = 0;
                (*index)++;
                while (true)
                {
                    string String2 = splitted[i];
                    if ((*index) + 1 < splitted.size() &&
                    (splitted[(*index) + 1] == "+" || splitted[(*index) + 1] == "-"))
                    {
                        string finalType;
                        string str_value = String2;
                        string type = getTypeVar(String2);
                        this->Operation(splitted, &str_value, &finalType, type, index);
                        FinalString += str_value;
                    }
                    else
                        FinalString += String2;
                    if (String2 == "[")
                        num2++;
                    else if (String2 == "]")
                    {
                        if (num2 <= 0)
                            break;
                        else
                            num2--;
                    }
                    (*index)++;
                    if ((*index) > splitted.size() - 1)
                        break;
                }
                //this->PrintError("Invalid variable: " + VariableName);
                //return;
                params.push_back(FinalString);
            }
        }
        else if (string_check == "]")
        {
            IsComparator = false;
        }
        else if (string_check == "=" && ((*index) + 1) < splitted.size() - 1)
        {
            if (splitted[(*index) + 1] == "=")
                Comparator = "==";
            else if (splitted[(*index) + 1] == ">")
                Comparator = "=>";
            else if (splitted[(*index) + 1] == "<")
                Comparator = "=<";
            else
                Comparator = string_check;
        }
        else if (string_check == ">" && ((*index) + 1) < splitted.size() - 1)
        {
            if (splitted[(*index) + 1] == "=")
                Comparator = ">=";
            else
                Comparator = string_check;
        }
        else if (string_check == "<" && ((*index) + 1) < splitted.size() - 1)
        {
            if (splitted[(*index) + 1] == "=")
                Comparator = "<=";
            else
                Comparator = string_check;
        }
        else if (string_check == "&" && ((*index) + 1) < splitted.size() - 1)
        {
            if (splitted[(*index) + 1] == "&")
                Comparator = "&&";
            else
                Comparator = string_check;
        }
        else if (string_check == "|" && ((*index) + 1) < splitted.size() - 1)
        {
            if (splitted[(*index) + 1] == "|")
                Comparator = "||";
            else
                Comparator = string_check;
        }
        else if (string_check == "!" && ((*index) + 1) < splitted.size() - 1)
        {
            if (splitted[(*index) + 1] == "=")
                Comparator = "!=";
            else
                Comparator = string_check;
        }
        else
        {
            IsComparator = false;
            if (IsNewFunc)
                parameters->push_back(string_check);
            else
                params.push_back(string_check);
        }
        if (IsComparator)
        {
            if (Comparator.length() == 2)
                (*index)++;
            LoadParamVariable(&params, IsNewFunc, parameters);
            parameters->push_back(Comparator);
            Comparator = "";
        }
        if (num <= 0 && string_check == ")")
            break;
        if ((*index) < splitted.size()-1)
            (*index)++;
        else
            break;
        string_check = splitted[(*index)];
    } while (true);

    LoadParamVariable(&params, IsNewFunc, parameters);
}