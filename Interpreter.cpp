#include "Interpreter.h"
#include "Utilities.h"
#include "Variable.h"
#include <iostream>

using namespace std;
using namespace Utilities;

Interpreter::Interpreter()
{
	this->typeVariables = {
		"int",
		"string"
	};
	this->FindindStaple = false;
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
		this->debugVariables();
		this->debugFunctions();
		for (int i = 0; i < this->VariablesInfos.size(); i++)
		{
			for (int i2 = 0; i2 < VariablesInfos[i].size(); i2++)
			{
				cout << "Variable in if " << i << " = " << VariablesInfos[i][i2].get_type() << endl;
			}
		}
		newfile.close();
	}
	else
	{
		cout << "F" << endl;
	}
}

void Interpreter::Line(string line)
{
	try
	{
		vector<string> splitted = split(line, ' ');
		for (int i = 0; i < splitted.size(); i++)
		{
			//cout << splitted[i] << endl;
		}
		//cout << splitted[0] << endl;
		//cout << splitted.size() << endl;
		string lastString;

		for (this->i = 0; this->i < splitted.size(); this->i++)
		{
			if (this->FindindStaple)
				this->FindGraffa(splitted);

			const string String = splitted[i];
			//cout << stringa << endl;
			//vector<char> Operatori = searchOperatori(stringa);
			//vector<string> Parole = OperatoriParole(stringa);
			bool FoundInVector = findInVector(typeVariables, lastString);

			if (!findInVector(typeVariables, String))
			{
				if (!FoundInVector)
				{
					if (String == "=")
					{
						//const
						string name = lastString;
						string type;
						Variable var = this->find_variable(name);
						if (var.get_type() == "")
						{
							if (!(i - 1 < 0))
							{
								if (findInVector(typeVariables, splitted[this->i - 2]))
								{
									type = splitted[this->i - 2];
								}
							}
						}
						else
						{
							type = var.get_type();
						}
						//cout << stringa << " e' l'operatore di assegnazione" << endl;
						if (type == "int")
						{
							this->loadIntVariable(splitted, name);
						}
						else if (type == "string")
						{
							this->loadStringVariable(splitted, name);
						}
					}
					else if (String == "+")
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
							//cout << "nome funzione = " << nomefunzione << endl;
							vector<string> parameters;
							string stringcheck = splitted[this->i];
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

							if (namefunction == "print")
							{
								this->print(parameters);
							}
							else if (namefunction == "if")
							{
								cout << "if statement" << endl;
								const string if1 = parameters[0];
								const string comparison = parameters[1];
								const string if2 = parameters[2];


								this->If(if1, comparison, if2);
							}
							else
							{
								Function func = this->find_function(namefunction);
								if (func.get_name() == "")
								{
									try
									{
										if (splitted[i-3] == "func")
										{
											cout << "Aggiungo funzione" << endl;
											Function func;
											func.setup(namefunction);
											this->functions.push_back(func);
										}
										else
										{
											cout << "Error: invalid function" << endl;
										}
									}
									catch (const exception&)
									{
										cout << "Error: invalid function" << endl;
									}
								}
								else
								{
									cout << "Funzione esistente" << endl;
								}
							}
						}
						else
						{
							cout << "non e' una funzione" << endl;
						}
					}
					else if (String == "{")
					{
						if (this->Ifs.size() > 0)
						{
							cout << "Inizio If" << endl;
							const int lastelem = Ifs.size() - 1;
							if (!this->Ifs[lastelem])
							{
								cout << "If falso" << endl;
								this->FindGraffa(splitted);
							}
						}
						else
						{
							cout << "Apertura procedure" << endl;
						}
					}
					else if (String == "}")
					{
						if (this->Ifs.size() > 0)
						{
							cout << "Chiusura if" << endl;
							
							Ifs.erase(Ifs.end() - 1);
							cout << VariablesInfos.size() << endl;
							this->VariablesInfos.erase(VariablesInfos.end() - 1);
							cout << VariablesInfos.size() << endl;
							//this->Ifs.erase(Ifs.size() - 1)
						}
						else
						{
							cout << "Chiusura funzione" << endl;
						}
					}
				}
			}
			//cout << endl;
			lastString = splitted[i];
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
	bool found = false;
	for (int i = 0; i < this->variables.size(); i++)
	{
		Variable* var = &this->variables[i];
		if (var->get_name() == name)
		{
			found = true;
			//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
			return *var;
		}
	}
	for (int i = 0; i < this->VariablesInfos.size(); i++)
	{
		for (int i2 = 0; i2 < this->VariablesInfos[i].size(); i2++)
		{
			Variable* var = &this->VariablesInfos[i][i2];
			if (var->get_name() == name)
			{
				found = true;
				//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
				return *var;
			}
		}
	}
	if (!found)
	{
		Variable var;
		//cout << var.get_str_value() << endl;
		return var;
	}
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
				if (Ifs.size() > size)
				{
					vector<Variable> vettore;
					VariablesInfos.push_back(vettore);
				}
				this->VariablesInfos[size-1].push_back(var);
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
				if (character == ' ')
					cout << " " << string(1, String[++i]);
				else
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

void Interpreter::FindGraffa(vector<string> splitted)
{
	bool found = false;
	int numopenedstaples = 0;
	do
	{
		if (this->i >= splitted.size()-1)
		{
			break;
		}
		this->i++;
		//cout << splitted[this->i] << " " << " " << numgraffeaperte << endl;
		if (splitted[this->i] == "{")
			numopenedstaples++;
		if (splitted[this->i] == "}")
			numopenedstaples--;
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