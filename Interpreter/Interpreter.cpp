#include "Interpreter.h"
#include "../Other/Utilities/Utilities.h"
#include "../Variable/Variable.h"
#include <iostream>

using namespace std;
using namespace Utilities;

void Interpreter::Setup()
{
	this->typeVariables = {
		"int",
		"string"
	};
	this->FindindStaple = false;
	this->writingFunc = false;
}

Interpreter::Interpreter()
{
	this->Setup();
}

Interpreter::Interpreter(vector<Variable> oldVariables)
{
	this->Setup();
	for (int i = 0; i < oldVariables.size(); i++)
	{
		this->variables.push_back(oldVariables[i]);
	}
	
}

void Interpreter::start(std::string nomefile)
{
	fstream newfile;
	newfile.open(nomefile, ios::in);
	if (newfile.is_open())
	{
		string tp;
		while (getline(newfile, tp))
		{
			this->Line(tp);
		}
		//this->debugVariables();
		//this->debugFunctions();
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
		cout << "Error: file not found" << endl;
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
			const string String = splitted[i];
			//cout << String << endl;

			if (String == "/")
			{
				if (splitted[i + 1] == "/")
				{
					i++;
					return;
				}
			}

			//cout << "writingFunc = " << writingFunc << endl;
			if (this->Ifs.size() == 0 && this->functions.size() > 0 && writingFunc)
			{
				//cout << "wtf " << String << endl;
				if (i == (splitted.size()-1))
				{
					//cout << "adding line" << endl;
					this->functions[functions.size() - 1].add_line(splitted);
					//return;
					//cout << this->functions[0].get_lines().size() << endl;
				}
			}

            if (this->Ifs.size() != 0)
            {
                if (!this->Ifs[Ifs.size()-1])
                {
                    if (String == "}")
                    {
                        this->Ifs.erase(this->Ifs.end() - 1);
                    }
                    else
                    {
                        return;
                    }
                }
            }
            /*
			if (this->FindindStaple)
				this->FindGraffa(splitted);
            */
			//cout << stringa << endl;
			//vector<char> Operatori = searchOperatori(stringa);
			//vector<string> Parole = OperatoriParole(stringa);
			bool FoundInVector = findInVector(typeVariables, lastString);

			if (!findInVector(typeVariables, String))
			{
				if (!FoundInVector)
				{
					if (String == "=" && !writingFunc)
					{
						//const
						string name = lastString;
						string type;
						Variable var = this->find_variable(name);
						if (var.get_type() == "")
						{
							int num = 0;
							while ((i - num) >= 0)
							{
								if (findInVector(typeVariables, splitted[this->i - num]))
								{
									type = splitted[this->i - num];
								}
								num++;
							}
						}
						else
						{
							type = var.get_type();
						}
						//cout << "adding a variable" << endl;
						if (type == "int")
						{
							//cout << "adding int var" << endl;
							this->loadIntVariable(splitted, name);
						}
						else if (type == "string")
						{
							//cout << "adding string var" << endl;
							this->loadStringVariable(splitted, name);
						}
						else
						{
							cout << "Error: invalid variable type" << endl;
						}
					}
					else if (String == "+" && !writingFunc)
					{
						//cout << lastStringa << " e' l'addendo 1" << endl;
						string add1 = lastString;
						string add2 = splitted[(++this->i)];

						Variable Var = this->find_variable(add1);
						const string type = Var.get_type();
						if (type != "")
						{
							//cout << "e' una variabile" << endl;
							if (type == "string")
								add1 = Var.get_str_value();
							else if (type == "int")
								add1 = to_string(Var.get_int_value());
						}

						Variable Var2 = this->find_variable(add2);
						const string type2 = Var2.get_type();
						if (type2 != "")
						{
							//cout << "e' una variabile" << endl;
							if (type2 == "string")
								add2 = Var2.get_str_value();
							else if (type2 == "int")
								add2 = to_string(Var2.get_int_value());
						}
						//cout << addendo1 << " e' l'addendo 1" << endl;
						//cout << addendo2 << " e' l'addendo 2" << endl;
						bool isNan1 = isNan(add1);
						bool isNan2 = isNan(add2);

						if (isNan1 || isNan2)
						{
							//cout << "Output: " << (addendo1 + addendo2) << endl;
							string value = add1 + add2;
							string typeFinalValue;
							this->AddStrings(splitted, &value, &typeFinalValue);
							//cout << "Output: " << valore << endl;
						}
						else
						{
							string value = to_string(stoi(add1) + stoi(add2));
							string typeFinalValue;
							this->AddIntegers(splitted, &value, &typeFinalValue);
							//cout << valore << endl;
							//cout << "Output: " << stoi(valore) << endl;
							//cout << "Output: " << (stoi(addendo1) + stoi(addendo2)) << endl;
						}
						//cout << splitted[(++i)] << " e' l'addendo 2" << endl;
					}
					else if (String == "(")
					{
						if (lastString != "")
						{
							const string namefunction = lastString;
							//cout << "function name = " << namefunction << endl;
							bool IsNewFunc = false;
							//cout << "i = " << i << endl;
							if ((i - 2) >= 0)
								if(splitted[i-2] == "func")
									IsNewFunc = true;
							//cout << IsNewFunc << endl;
							vector<string> parameters;
							//cout << parameters.size() << endl;
							string stringcheck = splitted[this->i];
							if (writingFunc)
							{
								int Integer = this->i;
								do
								{
									//cout << stringacheck << endl;
									if (stringcheck != "," && stringcheck != "(")
										if (stringcheck == "=" && splitted[Integer + 1] == "=")
										{
											parameters.push_back("==");
											Integer++;
										}
										else
											parameters.push_back(stringcheck);
										stringcheck = splitted[++Integer];
								} while (stringcheck != ")" || Integer >= splitted.size());
							}
							else
							{
								do
								{
									//cout << stringacheck << endl;
									if (stringcheck != "," && stringcheck != "(")
										if (stringcheck == "=" && splitted[this->i + 1] == "=")
										{
											parameters.push_back("==");
											this->i++;
										}
										else
											parameters.push_back(stringcheck);
										stringcheck = splitted[++this->i];
								} while (stringcheck != ")" || this->i >= splitted.size());
							}

							if (namefunction == "print" && !writingFunc)
							{
								this->print(parameters);
							}
							else if (namefunction == "if")
							{
								if (!writingFunc)
								{
									//cout << "if statement" << endl;
									const string if1 = parameters[0];
									const string comparison = parameters[1];
									const string if2 = parameters[2];


									this->If(if1, comparison, if2);
								}
								else
								{
									this->Ifs.push_back(true);
								}
							}
							else if (namefunction == "for")
							{
								cout << "for loop" << endl;
								int num = 0;
								vector<string> assign;
								vector<string> check;
								vector<string> advancing;
								for (int index = 0; index < parameters.size(); index++)
								{
									if (parameters[index] != ";")
									{
										if (num == 0)
										{
											assign.push_back(parameters[index]);
										}
										else if (num == 1)
										{
											check.push_back(parameters[index]);
										}
										else if (num == 2)
										{
											advancing.push_back(parameters[index]);
										}
										else
											break;
									}
									else
									{
										num++;
									}
								}
								
								this->ForLoop(assign, check, advancing);
							}
							else if (namefunction == "while")
							{
								this->WhileLoop(parameters);
							}
							else if (!writingFunc)
							{
								Function func = this->find_function(namefunction);
								if (func.get_name() == "")
								{
									try
									{
										if (IsNewFunc)
										{
											//cout << "Aggiungo funzione" << endl;
											Function func;
											func.setup(namefunction);
											//writingFunc = true;
											this->functions.push_back(func);
											//cout << functions.size() << endl;
										}
										else
										{
											cout << "Error: invalid function" << endl;
										}
									}
									catch (const exception& error)
									{
										//cout << error.what() << endl;
										cout << "Error: invalid function" << endl;
									}
								}
								else
								{
									vector<vector<string>> lines = func.get_lines();
									Interpreter interpreter(this->variables);
									for (int i = 0; i < lines.size(); i++)
									{
										vector<string> line = lines[i];
										string Strline = "";
										for (int i2 = 0; i2 < line.size(); i2++)
										{
											Strline += (line[i2] + " ");
										}
										//cout << Strline << endl;
										interpreter.Line(Strline);
									}
									
									//cout << "Funzione esistente" << endl;
								}
							}
						}
						else
						{
							cout << "non e' una funzione" << endl;
						}
					}
					else if (String == "{" && !writingFunc)
					{
						if (this->Ifs.size() > 0)
						{
							//cout << "Inizio If" << endl;
							const int lastelem = Ifs.size() - 1;
							if (!this->Ifs[lastelem])
							{
								cout << "If falso" << endl;
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
							//cout << "Chiusura if" << endl;
							Ifs.erase(Ifs.end() - 1);
							//cout << VariablesInfos.size() << endl;
							if (VariablesInfos.size() > 0)
								this->VariablesInfos.erase(VariablesInfos.end() - 1);

							if (writingFunc && i == (splitted.size()-1))
								this->functions[functions.size() - 1].add_line(splitted);
							//cout << VariablesInfos.size() << endl;
							//this->Ifs.erase(Ifs.size() - 1)
						}
						else
						{
							if (this->functions.size() > 0)
							{
								//functions.erase(functions.end() - 1);
								//cout << VariablesInfos.size() << endl;
								//this->VariablesInfos.erase(VariablesInfos.end() - 1);
								writingFunc = false;
								//cout << "Chiusura funzione" << endl;
							}
							else
							{
								//cout << "Chiusura procedure" << endl;
							}
						}
					}
				}
			}
			//cout << endl;
			lastString = splitted[i];
			//line = String;
			if (i == (splitted.size()-1) && writingFunc)
			{
				//cout << String << endl;
				//cout << (String == "{" || String == "}" || String == ")") << " " << (this->Ifs.size() != 0) << endl;
				if((String == "{" || String == "}" || String == ")") && (this->Ifs.size() != 0))
				{
					//cout << "adding " << String << endl;
					//this->functions[functions.size() - 1].add_line();
					this->functions[functions.size() - 1].add_line(splitted);
				}
				//cout << "adding line" << endl;
				//cout << this->functions[0].get_lines().size() << endl;
			}
			/**/
		}
		/*if (this->Ifs.size() == 0 && this->FindindGraffa)
		{
			cout << "Error: expected { after if statement" << endl;
			return;
		}*/
		//cout << lastStringa << endl;
	}
	catch (const exception& error)
	{
		cout << error.what() << endl;
	}
	//cout << endl;
}

Variable Interpreter::find_variable(string name)
{
	//bool found = false;
	Variable VAR;
	for (int i = 0; i < this->variables.size(); i++)
	{
		Variable* var = &this->variables[i];
		if (var->get_name() == name)
		{
			//found = true;
			//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
			VAR = *var;
			break;
			//return *var;
		}
	}
	for (int i = 0; i < this->VariablesInfos.size(); i++)
	{
		for (int i2 = 0; i2 < this->VariablesInfos[i].size(); i2++)
		{
			Variable* var = &this->VariablesInfos[i][i2];
			if (var->get_name() == name)
			{
				//found = true;
				//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
				VAR = *var;
				break;
				//return *var;
			}
		}
	}
	return VAR;
}

Variable* Interpreter::find_variable_pointer(string name)
{
	//bool found = false;
	Variable var;
	Variable* VAR = &var;
	for (int i = 0; i < this->variables.size(); i++)
	{
		Variable* var = &this->variables[i];
		if (var->get_name() == name)
		{
			//found = true;
			//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
			VAR = var;
			break;
			//return *var;
		}
	}
	for (int i = 0; i < this->VariablesInfos.size(); i++)
	{
		for (int i2 = 0; i2 < this->VariablesInfos[i].size(); i2++)
		{
			Variable* var = &this->VariablesInfos[i][i2];
			if (var->get_name() == name)
			{
				//found = true;
				//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
				static Variable* VAR = var;
				break;
				//return *var;
			}
		}
	}
	return VAR;
}

void Interpreter::loadIntVariable(vector<string> splitted, string name)
{
	string typeFinalValue;
	string value = splitted[(++this->i)];
	Variable Var = this->find_variable(value);
	const string type = Var.get_type();
	if (type != "")
	{
		//cout << "e' una variabile" << endl;
		if (type == "string")
			value = Var.get_str_value();
		else if (type == "int")
			value = to_string(Var.get_int_value());
	}
	this->AddIntegers(splitted, &value, &typeFinalValue);
	if (typeFinalValue == "invalid")
	{
		return;
	}
	//cout << valore << " e' il valore della variabile" << endl;
	if (!isNan(string(1, (name[0]))))
	{
		cout << "Error: a variable's name cannot start with a number" << endl;
	}
	else
	{
		bool found = false;
		for (int i = 0; i < this->variables.size(); i++)
		{
			Variable* var = &this->variables[i];
			if (var->get_name() == name)
			{
				var->set_value(stoi(value));
				found = true;
				break;
			}
		}
		if (!found)
		{
			Variable var;
			var.setup(name, stoi(value));
			if (this->Ifs.size() > 0)
			{
				int size = VariablesInfos.size();
				//cout << size << " " << Ifs.size() << endl;
				if (Ifs.size() > size)
				{
					//cout << "ok" << endl;
					vector<Variable> vettore;
					VariablesInfos.push_back(vettore);
				}
				//cout << this->VariablesInfos.size() << " " << Ifs.size() << endl;
				this->VariablesInfos[(this->VariablesInfos.size() - 1)].push_back(var);
			}
			else
				this->variables.push_back(var);
		}
	}

	//cout << "Output: " << var.get_int_value() << endl;
}

void Interpreter::loadStringVariable(vector<string> splitted, string name)
{
	string typeFinalValue;
	string value = splitted[(++this->i)];
	
	this->AddStrings(splitted, &value, &typeFinalValue);
	//cout << valore << " e' il valore della variabile" << endl;
	if (!isNan(string(1, (name[0]))))
	{
		cout << "Error: a variable's name cannot start with a number" << endl;
	}
	else
	{
		bool found = false;
		for (int i = 0; i < this->variables.size(); i++)
		{
			Variable* var = &this->variables[i];
			if (var->get_name() == name)
			{
				var->set_value(value);
				found = true;
				break;
			}
		}
		if (!found)
		{
			Variable var;
			var.setup(name, value);
			if (this->Ifs.size() > 0)
			{
				int size = VariablesInfos.size();
				if (VariablesInfos.size() <= size)
				{
					vector<Variable> vettore;
					VariablesInfos.push_back(vettore);
				}
				this->VariablesInfos[size].push_back(var);
			}
			else
				this->variables.push_back(var);
		}
	}

	//cout << "Output: " << var.get_int_value() << endl;
}

void Interpreter::printString(string String)
{
	for (int i = 0; i < String.length(); i++)
	{
		char character = String[i];
		if (character != '"')
		{
			if (character == '\\')
			{
				string parola = (string(1, character) + string(1, String[++i]));
				//cout << endl << "parola = " << parola << (parola == "\\n") << endl;
				if (parola == "\\n")
				{
					cout << endl;
				}
			}
			else
			{
				//cout << "carattere = " << carattere << " " << (carattere == ' ') << endl;
				cout << character;
			}
		}
	}
}

void Interpreter::AddIntegers(std::vector<std::string> splitted, string* value, string* typeFinalValue)
{
	do
	{
		//cout << valore << endl;
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
				//cout << "e' una variabile" << endl;
				if (type == "string")
					add2 = var.get_str_value();
				else if (type == "int")
					add2 = to_string(var.get_int_value());
			}
			//cout << addendo1 << " e' l'addendo 1" << endl;
			//cout << addendo2 << " e' l'addendo 2" << endl;
			bool isNan1 = isNan(add1);
			bool isNan2 = isNan(add2);

			//cout << isNan1 << " " << isNan2 << endl;

			if (isNan1 || isNan2)
			{
				cout << "Error: invalid type of value" << endl;
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
				//cout << "risultato = " << risultato << endl;
				*value = to_string(risultato);
				//cout << "addendo1 = " << addendo1 << endl;
				//cout << "addendo2 = " << addendo2 << endl;
				*typeFinalValue = "int";
				//valore = (stoi(addendo1) + stoi(addendo2));
				//cout << "valore here: " << valore << endl;
				//cout << "output: " << valore << endl;
			}
		}
		//cout << "fine" << endl;
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
			//cout << "risultato = " << newRisultato << endl;
			*value = newResult;
			//cout << "addendo1 = " << addendo1 << endl;
			//cout << "addendo2 = " << addendo2 << endl;
			*typeFinalValue = "string";
			//cout << addendo1 << " e' l'addendo 1" << endl;
			//cout << addendo2 << " e' l'addendo 2" << endl;

			//cout << isNan1 << " " << isNan2 << endl;
		}
	} while (true);
}

void Interpreter::If(const string if1, const string comparison, const string if2)
{
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
	else
	{
		Variable var1 = this->find_variable(if1);

		if (var1.get_type() == "")
		{
			cout << "Error: variable not found" << endl;
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
	else
	{
		Variable var2 = this->find_variable(if2);
		if (var2.get_type() == "")
		{
			cout << "Error: variable not found" << endl;
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
	}

	if (comparison == "==")
	{

		if (Val1 == Val2)
		{
			//cout << "true" << endl;
			this->Ifs.push_back(true);
		}
		else
		{
			//cout << "false" << endl;
			this->Ifs.push_back(false);
		}
	}
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

void Interpreter::WhileLoop(const vector<string> condition)
{
	//Check
	string VariableNameCheck;
	string Comparation;
	string ComparationValue;

	vector<string> Comparations = {
		"==",
		">",
		"<"
	};

	for (int index = 0; index < condition.size(); index++)
	{
		string String = condition[index];
		cout << String << endl;
		if (findInVector(Comparations, String))
		{
			//cout << "Simbolo" << endl;
			Comparation += String;
			int num = 1;
			if (!findInVector(Comparations, condition[index - 1]))
				VariableNameCheck = condition[index - 1];
			if (!findInVector(Comparations, condition[index + 1]))
				ComparationValue = condition[++index];
		}
	}

	cout << VariableNameCheck << " " << Comparation << " " << ComparationValue << endl;
	Variable* var1 = this->find_variable_pointer(VariableNameCheck);
	Variable* var2 = this->find_variable_pointer(ComparationValue);
	string Type1 = var1->get_type();
	if (Comparation == "==")
	{
		if (Type1 == "int")
		{
			while (string(1, var1->get_int_value()) == ComparationValue)
			{
				cout << "While" << endl;
			}
		}
	}
}

void Interpreter::FindGraffa(vector<string> splitted)
{
	bool found = false;
	int numopenedstaples = 0;
	do
	{
        cout << this->i << " " << splitted.size() << endl;
		if (this->i >= splitted.size()-1)
		{
			break;
		}
        cout << "cock " << splitted[this->i] << endl;
		cout << splitted[this->i] << " " << " " << numopenedstaples << endl;
		if (splitted[this->i] == "{")
			numopenedstaples++;
		if (splitted[this->i] == "}")
			numopenedstaples--;
        this->i++;
        cout << endl;
	} while (splitted[this->i] != "}" && numopenedstaples > 0);
	if (splitted[this->i] == "}" && numopenedstaples == 0)
		found = true;
	//cout << "trovata = " << trovata << endl;

	this->FindindStaple = !found;

	if (found)
	{
		this->Ifs.erase(this->Ifs.end() - 1);
		//this->FindindGraffa = false;
	}
	/*for (int i = 0; i < this->Ifs.size(); i++)
	{
		cout << "Ifs " << i << " " << Ifs[i] << endl;
	}*/
}