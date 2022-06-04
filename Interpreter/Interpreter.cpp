#include "Interpreter.h"
#include "../Other/Utilities/Utilities.h"
#include "../Variable/Variable.h"
#include "../While/While.h"
#include <iostream>

using namespace std;
using namespace Utilities;
using namespace interpreter;

void Interpreter::Setup()
{
	this->typeVariables = {
		"var"
	};
    this->line = 0;
	this->FindindStaple = false;
    this->FindingElse = false;
	this->writingFunc = false;
    this->writingWhile = false;
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

Interpreter::Interpreter(vector<Variable> oldVariables, bool executingFunction, Function* func)
{
	this->Setup();
    this->isExecutingFunc = executingFunction;
    this->FUNC = func;
	for (int i = 0; i < oldVariables.size(); i++)
	{
		this->variables.push_back(oldVariables[i]);
	}
	
}

void Interpreter::start(std::string nomefile, bool debug)
{
	fstream newfile;
	newfile.open(nomefile, ios::in);
	if (newfile.is_open())
	{
		string tp;
		while (getline(newfile, tp))
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
		newfile.close();
	}
	else
	{
		this->PrintError("File not found");
	}
}

void Interpreter::Line(string line)
{
	try
	{
		vector<string> splitted = split(line, ' ');
		//cout << "printing splitted members" << endl;
		for (int i = 0; i < splitted.size(); i++)
		{
			//cout << splitted[i] << endl;
		}
		//cout << splitted[0] << endl;
		//cout << splitted.size() << endl;
		//string line;
		string lastString;
		//bool writingFunc = false;
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
				if (splitted[i + 1] == "/")
				{
					i++;
					return;
				}
			}

			if (this->Ifs.size() == 0)
			{
                if (i == (splitted.size()-1))
                {
                    if (this->functions.size() > 0 && writingFunc)
                        this->functions[functions.size() - 1].add_line(splitted);
                    else if (this->whiles.size() > 0 && writingWhile)
                        this->whiles[whiles.size() - 1].add_line(splitted);
                }
			}

            if (this->Ifs.size() != 0)
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
            /*
			if (this->FindindStaple)
				this->FindGraffa(splitted);
            */
			//vector<char> Operatori = searchOperatori(stringa);
			//vector<string> Parole = OperatoriParole(stringa);

            if (this->writingList.size() > 0 && this->writingList[writingList.size()-1])
            {
                if (String == "]")
                {
                    this->writingList.erase(this->writingList.end() - 1);
                    if (this->writingList.size() == 0)
                        this->printList(this->ListWriting[0]);
                }
                else
                {
                    this->loadList(splitted, true, &i);
                }
            }

			bool FoundInVector = findInVector(typeVariables, lastString);
            bool CheckWriting = writingFunc || writingWhile;

			if (!findInVector(typeVariables, String) && this->writingList.size() <= 0)
			{
                //cout << "Entering with " << String << endl;
				if (!FoundInVector)
				{
					if (String == "=" && !CheckWriting)
					{
						//const
						string name = lastString;
						this->loadVariable(splitted, name);
					}
					else if (String == "+" && !CheckWriting)
					{
						string add1 = lastString;
						string add2 = splitted[(++this->i)];

						Variable Var = this->find_variable(add1);
						const string type = Var.get_type();
						if (type != "")
						{
							if (type == "string")
								add1 = Var.get_str_value();
							else if (type == "int")
								add1 = to_string(Var.get_int_value());
						}

						Variable Var2 = this->find_variable(add2);
						const string type2 = Var2.get_type();
						if (type2 != "")
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
                        //cout << "entered" << endl;
						if (lastString == "")
                        {
                            this->PrintError("Not a function");
                            return;
                        }
                        const string namefunction = lastString;
                        bool IsNewFunc = false;

                        if ((i - 2) >= 0)
                            if(splitted[i-2] == "func")
                                IsNewFunc = true;

                        vector<string> parameters;

                        int num = 0;

                        this->i++;

                        string stringcheck = splitted[this->i];
                        /*for (int j = 0; j < splitted.size(); ++j) {
                            cout << splitted[j] << endl;
                        }*/

                        if (CheckWriting)
                        {
                            int Integer = this->i;
                            do
                            {
                                if (Integer >= splitted.size()) break;

                                if (stringcheck != ",")
                                {
                                    if (stringcheck == "(")
                                    {
                                        string FunctionName = splitted[Integer-1];
                                        vector<string> params;
                                        Integer++;
                                        while (true)
                                        {
                                            string param = splitted[Integer];
                                            if (param == ")")
                                            {
                                                num--;
                                                break;
                                            }
                                            if (param != ",")
                                                params.push_back(param);
                                            if (Integer >= splitted.size()-1)
                                                break;
                                            Integer++;
                                        }
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
                                            Variable returned = this->executeFunction(FunctionName, CheckWriting, params, IsNewFunc, &Returning);
                                            parameters[parameters.size()-1] = returned.get_value();
                                        }
                                    }
                                    else if (stringcheck == ")")
                                    {
                                        if (num <= 0)
                                            break;
                                    }
                                    else if (stringcheck == "=" && (Integer+1) < splitted.size() && splitted[Integer + 1] == "=")
                                    {
                                        parameters.push_back("==");
                                        Integer++;
                                    }
                                    else if (stringcheck == "&" && (Integer+1) < splitted.size()-1)
                                    {
                                        if (splitted[this->i + 1] == "&")
                                        {
                                            parameters.push_back("&&");
                                            this->i++;
                                        }
                                    }
                                    else if (stringcheck == "|" && (Integer+1) < splitted.size()-1)
                                    {
                                        if (splitted[this->i + 1] == "|")
                                        {
                                            parameters.push_back("||");
                                            this->i++;
                                        }
                                    }
                                    else
                                        parameters.push_back(stringcheck);
                                }
                                if (num <= 0 && stringcheck == ")")
                                    break;
                                if (Integer < splitted.size()-1)
                                    Integer++;
                                else
                                    break;
                                stringcheck = splitted[Integer];
                            } while (true);
                        }
                        else
                        {
                            do
                            {
                                if (this->i >= splitted.size()) break;
                                //cout << "StringCheck = " << stringcheck << endl;
                                //cout << splitted[this->i-1] << endl;
                                //cout << "num = " << num << endl;
                                //cout << "i = " << this->i << endl;
                                //cout << "splitted size = " << splitted.size() << endl;
                                if (stringcheck != ",")
                                {
                                    if (stringcheck == "(")
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
                                            Variable returned = this->executeFunction(FunctionName, CheckWriting, params, IsNewFunc, &Returning);
                                            //cout << "value = " << returned.get_value() << endl;
                                            parameters[parameters.size()-1] = returned.get_value();
                                        }
                                        //num++;
                                    }
                                    else if (stringcheck == ")")
                                    {
                                        if (num <= 0)
                                            break;
                                    }
                                    else if (stringcheck == "[")
                                    {
                                        //Array member
                                        string VariableName = splitted[i-1];

                                        Variable var = this->find_variable(VariableName);
                                        if (var.get_type() != "")
                                        {
                                            //cout << VariableName << endl;
                                            if (i+1 < splitted.size())
                                            {
                                                string index = splitted[i+1];
                                                if (index == "]")
                                                {
                                                    this->PrintError("No index");
                                                    return;
                                                }
                                                if (getTypeVar(index) != "int")
                                                {
                                                    this->PrintError("Invalid index type");
                                                    return;
                                                }
                                                i++;
                                                parameters.erase(parameters.end() - 1);
                                                //cout << (var.get_list_value()[stoi(index)]).get_type() << endl;
                                                Variable member = var.get_list_value()[stoi(index)];
                                                string type = member.get_type();
                                                if (type == "string")
                                                    parameters.push_back(member.get_str_value());
                                                else if (type == "int")
                                                    parameters.push_back(to_string(member.get_int_value()));
                                                else if (type == "bool")
                                                    parameters.push_back(to_string(member.get_bool_value()));
                                                else if (type == "list")
                                                    parameters.push_back(GetListValue(member));
                                            }
                                        }
                                        else if (parameters.size() > 0 && getTypeVar(parameters[parameters.size()-1]) == "list")
                                        {
                                            //member inside a list
                                            string index = splitted[i+1];
                                            if (index == "]")
                                            {
                                                this->PrintError("No index");
                                                return;
                                            }
                                            if (getTypeVar(index) != "int")
                                            {
                                                this->PrintError("Invalid index type");
                                                return;
                                            }
                                            i++;
                                            //parameters.erase(parameters.end() - 1);
                                            //cout << (var.get_list_value()[stoi(index)]).get_type() << endl;
                                            int index2 = 0;
                                            this->writingList.push_back(true);
                                            this->loadList(split(parameters[parameters.size()-1], ' '), false, &index2);

                                            Variable member = this->ListWriting[0].get_list_value()[stoi(index)];
                                            string type = member.get_type();
                                            parameters.erase(parameters.end()-1);
                                            if (type == "string")
                                                parameters.push_back(member.get_str_value());
                                            else if (type == "int")
                                                parameters.push_back(to_string(member.get_int_value()));
                                            else if (type == "bool")
                                                parameters.push_back(to_string(member.get_bool_value()));
                                            else if (type == "list")
                                                parameters.push_back(GetListValue(member));
                                            this->ListWriting.erase(ListWriting.end()-1);

                                            while (this->writingList.size() != 0)
                                            {
                                                this->writingList.erase(writingList.end()-1);
                                            }
                                        }
                                        else
                                        {
                                            string FinalString = "[";
                                            int num2 = 0;
                                            i++;
                                            while (true)
                                            {
                                                string String2 = splitted[i];
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
                                                i++;
                                                if (i > splitted.size()-1)
                                                    break;
                                            }
                                            //this->PrintError("Invalid variable: " + VariableName);
                                            //return;
                                            parameters.push_back(FinalString);
                                        }
                                    }
                                    else if (stringcheck == "]")
                                    {

                                    }
                                    else if (stringcheck == "=" && (this->i+1) < splitted.size()-1)
                                    {
                                        if (splitted[this->i + 1] == "=")
                                        {
                                            parameters.push_back("==");
                                            this->i++;
                                        }
                                        else if (splitted[this->i + 1] == ">")
                                        {
                                            parameters.push_back("=>");
                                            this->i++;
                                        }
                                        else if (splitted[this->i + 1] == "<")
                                        {
                                            parameters.push_back("=<");
                                            this->i++;
                                        }
                                        else
                                            parameters.push_back(stringcheck);
                                    }
                                    else if (stringcheck == ">" && (this->i+1) < splitted.size()-1)
                                    {
                                        if (splitted[this->i + 1] == "=")
                                        {
                                            parameters.push_back(">=");
                                            this->i++;
                                        }
                                        else
                                            parameters.push_back(stringcheck);
                                    }
                                    else if (stringcheck == "<" && (this->i+1) < splitted.size()-1)
                                    {
                                        if (splitted[this->i + 1] == "=")
                                        {
                                            parameters.push_back("<=");
                                            this->i++;
                                        }
                                        else
                                            parameters.push_back(stringcheck);
                                    }
                                    else if (stringcheck == "&" && (this->i+1) < splitted.size()-1)
                                    {
                                        if (splitted[this->i + 1] == "&")
                                        {
                                            parameters.push_back("&&");
                                            this->i++;
                                        }
                                        else
                                            parameters.push_back(stringcheck);
                                    }
                                    else if (stringcheck == "|" && (this->i+1) < splitted.size()-1)
                                    {
                                        if (splitted[this->i + 1] == "|")
                                        {
                                            parameters.push_back("||");
                                            this->i++;
                                        }
                                        else
                                            parameters.push_back(stringcheck);
                                    }
                                    else
                                        parameters.push_back(stringcheck);
                                }
                                if (num <= 0 && stringcheck == ")")
                                    break;
                                if (this->i < splitted.size()-1)
                                    this->i++;
                                else
                                    break;
                                stringcheck = splitted[this->i];
                            } while (true);
                            /*cout << "printing params" << endl;
                            for (int j = 0; j < parameters.size(); ++j)
                            {
                                cout << "param = " << parameters[j] << endl;
                            }
                            cout << "finished" << endl;*/
                        }

                        bool Returning;
                        this->executeFunction(namefunction, CheckWriting, parameters, IsNewFunc, &Returning);
                        //cout << i << " " << splitted.size() << endl;
					}
					else if (String == "{" && !CheckWriting)
					{
						if (this->Ifs.size() > 0)
						{
							//cout << "Inizio If" << endl;
							const int lastelem = Ifs.size() - 1;
							if (!this->Ifs[lastelem])
							{
								//cout << "If falso" << endl;
								//this->FindGraffa(splitted);
							}
						}
						else
						{
							if (this->functions.size() > 0)
							{
								writingFunc = true;
								//cout << "Apertura funzione" << endl;
							}
							else
							{
								//cout << "Apertura procedure" << endl;
							}
						}
					}
					else if (String == "}")
					{
						if (this->Ifs.size() > 0)
						{
                            this->FindingElse = true;
                            this->FindingFromLine = this->line;
							if (VariablesInfos.size() > 0)
								this->VariablesInfos.erase(VariablesInfos.end() - 1);

							if (writingFunc && i == (splitted.size()-1))
								this->functions[functions.size() - 1].add_line(splitted);
                            if (writingWhile && i == (splitted.size()-1))
                                this->whiles[whiles.size() - 1].add_line(splitted);
						}
						else
						{
							if (this->functions.size() > 0 && writingFunc)
							{
								writingFunc = false;
							}
                            else if (this->whiles.size() > 0)
                            {
                                writingWhile = false;
                                this->whiles[whiles.size()-1].execute(this->variables);
                            }
							else
							{
								//cout << "Chiusura procedure" << endl;
							}
						}
					}
				}
			}

            if (i == splitted.size())
                i = splitted.size() - 1;

            lastString = splitted[i];

			if (i == (splitted.size()-1))
			{
                if (writingFunc)
                {
                    if((String == "{" || String == "}" || String == ")") && (this->Ifs.size() != 0))
                    {
                        this->functions[functions.size() - 1].add_line(splitted);
                    }
                }
                else if (writingWhile)
                {
                    if((String == "{" || String == "}" || String == ")") && (this->Ifs.size() != 0))
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
		if ((*j + 1) >= splitted.size() || splitted[*j + 1] == "")
			break;

		if (splitted[*j + 1] == "+")
		{
            bool notError = this->Addition(value, typeFinalValue, splitted, type1, j);
            if (!notError)
                break;
		}
        else if (splitted[*j + 1] == "-")
        {
            bool notError = this->Subtraction(value, typeFinalValue, splitted, type1, j);
            if (!notError)
                break;
        }
	} while (true);
}

void Interpreter::AddIntegers(std::vector<std::string> splitted, string* value, string* typeFinalValue)
{
	do
	{
		if ((this->i + 1) >= splitted.size() || splitted[this->i + 1] == "")
			break;
		if (splitted[this->i + 1] == "+")
		{
			const string add1 = *value;
			string add2 = splitted[(this->i += 2)];

			Variable var = this->find_variable(add2);
			const string type = var.get_type();
			if (type != "")
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
				/*
				valore = (addendo1 + addendo2);
				typeValoreFinale = "string";
				cout << "output: " << valore << endl;
				*/
			}
			else
			{
				int risultato = stoi(add1) + stoi(add2);

				*value = to_string(risultato);
				*typeFinalValue = "int";
			}
		}
	} while (true);
}

void Interpreter::AddStrings(std::vector<std::string> splitted, string* value, string* typeFinalValue)
{
	do
	{
		if ((this->i + 1) >= splitted.size() || splitted[this->i + 1] == "")
			break;
		if (splitted[this->i + 1] == "+")
		{
			const string add1 = *value;
			const string add2 = splitted[(this->i += 2)];
			string result = add1 + add2;
			string newResult;
			for (int i = 0; i < result.size(); i++)
			{
				if (i == 0 || i == result.size() - 1)
				{
					newResult += '"';
				}
				else
				{
					const char carattere = result[i];
					if (carattere != '"')
						newResult += carattere;
				}
			}
			*value = newResult;
			*typeFinalValue = "string";
		}
	} while (true);
}

void Interpreter::If(vector<string> parameters) //const string if1, const string comparison, const string if2
{
    vector<vector<string>> conditions = {{}};

    vector<string> ValidOperators1 = {
            "&&",
            "||"
    };

    vector<string> operators;
    int conditionnum = 0;
    for (int j = 0; j < parameters.size(); ++j) {
        string param = parameters[j];
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
        this->PrintError("No conditions");
        this->Ifs.push_back(false);
        return;
    }

    vector<bool> Checks = {};
    int num = 0;

    vector<string> ValidOperators = {
            "==",
            ">=",
            "=>",
            "<=",
            "=<"
    };

    for (int j = 0; j < conditions.size(); ++j) {
        vector<string> Condition = conditions[j];

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

            if (var1.get_type() == "")
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
            if (var2.get_type() == "")
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

    this->Ifs.push_back(Checks[0]);
}

void Interpreter::ForLoop(const vector<string> assign, const vector<string> check, const vector<string> advancing)
{
	string type = "";
	string variableName;
	string assignment;
	string value;
	for (int i = 0; i < assign.size(); i++)
	{
		string String = assign[i];
		if (findInVector(this->typeVariables, String))
		{
			type = String;
		}
		else if (String == "=")
		{
			assignment = String;
			variableName = assign[i-1];
		}
		if (type != "" && variableName != "" && assignment != "" && String != "=")
		{
			value += (String + " ");
		}
	}
	cout << type << " " << variableName << " " << assignment << " " << value << endl;

	//Check
	string VariableNameCheck;
	string Comparation;
	string ComparationValue;

	vector<string> Comparations = {
		"=",
		">",
		"<"
	};

	for (int index = 0; index < check.size(); index++)
	{
		string String = check[index];
		if (findInVector(Comparations, String))
		{
			//cout << "Simbolo" << endl;
			Comparation += String;
			int num = 1;
			if (!findInVector(Comparations, check[index-1]))
				VariableNameCheck = check[index-1];
			if (!findInVector(Comparations, check[index+1]))
				ComparationValue = check[++index];
		}
	}

	cout << VariableNameCheck << " " << Comparation << " " << ComparationValue << endl;

	string VarName;
	string Operator;

	vector<string> Operators = {
		"+",
		"-"
	};

	for (int index = 0; index < advancing.size(); index++)
	{
		string String = advancing[index];
		if (findInVector(Operators, String))
		{
			//cout << "Simbolo" << endl;
			Operator += String;
			int num = 1;
			if (!findInVector(Operators, advancing[index-1]))
				VarName = advancing[index-1];
		}
	}

	cout << VarName << " " << Operator << endl;

	Variable varDeclared = this->find_variable(variableName);
	Variable varCondition1 = this->find_variable(VariableNameCheck);
	
	if (Comparation == "<")
	{
		string typeVarDeclared = varDeclared.get_type();
		string typeVarCondition1 = varCondition1.get_type();
		if (typeVarDeclared == "int")
		{

			for (int i = 0; i < 10; i++)
			{
				cout << "lmao" << endl;
			}
		}
	}
}