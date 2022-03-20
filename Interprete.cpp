#include "Interprete.h"
#include "Utilities.h"
#include "Variable.h"
#include <iostream>

using namespace std;
using namespace Utilities;

Interprete::Interprete()
{
	this->tipiVariabili = {
		"int",
		"string"
	};
	this->FindindGraffa = false;
}

void Interprete::start(std::string nomefile)
{
	fstream newfile;
	newfile.open(nomefile, ios::in);
	if (newfile.is_open())
	{
		string tp;
		while (getline(newfile, tp))
		{
			this->Linea(tp);
		}
		this->debugVariables();
		this->debugFunctions();
		for (int i = 0; i < this->VariablesInIfs.size(); i++)
		{
			for (int i2 = 0; i2 < VariablesInIfs[i].size(); i2++)
			{
				cout << "Variable in if " << i << " = " << VariablesInIfs[i][i2].get_type() << endl;
			}
		}
		newfile.close();
	}
	else
	{
		cout << "F" << endl;
	}
}

void Interprete::Linea(string linea)
{
	try
	{
		vector<string> splitted = split(linea, ' ');
		for (int i = 0; i < splitted.size(); i++)
		{
			//cout << splitted[i] << endl;
		}
		//cout << splitted[0] << endl;
		//cout << splitted.size() << endl;
		string lastStringa;

		for (this->i = 0; this->i < splitted.size(); this->i++)
		{
			if (this->FindindGraffa)
				this->FindGraffa(splitted);

			const string stringa = splitted[i];
			//cout << stringa << endl;
			//vector<char> Operatori = searchOperatori(stringa);
			//vector<string> Parole = OperatoriParole(stringa);
			bool FoundInVector = findInVector(tipiVariabili, lastStringa);

			if (!findInVector(tipiVariabili, stringa))
			{
				if (!FoundInVector)
				{
					if (stringa == "=")
					{
						//const
						string name = lastStringa;
						string type;
						Variable var = this->find_variable(name);
						if (var.get_type() == "")
						{
							if (!(i - 1 < 0))
							{
								if (findInVector(tipiVariabili, splitted[this->i - 2]))
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
					else if (stringa == "+")
					{
						//cout << lastStringa << " e' l'addendo 1" << endl;
						string addendo1 = lastStringa;
						string addendo2 = splitted[(++this->i)];

						Variable Var = this->find_variable(addendo1);
						const string type = Var.get_type();
						if (type != "")
						{
							//cout << "e' una variabile" << endl;
							if (type == "string")
								addendo1 = Var.get_str_value();
							else if (type == "int")
								addendo1 = to_string(Var.get_int_value());
						}

						Variable Var2 = this->find_variable(addendo2);
						const string type2 = Var2.get_type();
						if (type2 != "")
						{
							//cout << "e' una variabile" << endl;
							if (type2 == "string")
								addendo2 = Var2.get_str_value();
							else if (type2 == "int")
								addendo2 = to_string(Var2.get_int_value());
						}
						//cout << addendo1 << " e' l'addendo 1" << endl;
						//cout << addendo2 << " e' l'addendo 2" << endl;
						bool isNan1 = isNan(addendo1);
						bool isNan2 = isNan(addendo2);

						if (isNan1 || isNan2)
						{
							//cout << "Output: " << (addendo1 + addendo2) << endl;
							string valore = addendo1 + addendo2;
							string typeValoreFinale;
							this->AddStringhe(splitted, &valore, &typeValoreFinale);
							//cout << "Output: " << valore << endl;
						}
						else
						{
							string valore = to_string(stoi(addendo1) + stoi(addendo2));
							string typeValoreFinale;
							this->AddInteri(splitted, &valore, &typeValoreFinale);
							//cout << valore << endl;
							//cout << "Output: " << stoi(valore) << endl;
							//cout << "Output: " << (stoi(addendo1) + stoi(addendo2)) << endl;
						}
						//cout << splitted[(++i)] << " e' l'addendo 2" << endl;
					}
					else if (stringa == "(")
					{
						if (lastStringa != "")
						{
							const string nomefunzione = lastStringa;
							//cout << "nome funzione = " << nomefunzione << endl;
							vector<string> parametri;
							string stringacheck = splitted[this->i];
							do
							{
								//cout << stringacheck << endl;
								if (stringacheck != "," && stringacheck != "(")
									if (stringacheck == "=" && splitted[this->i + 1] == "=")
									{
										parametri.push_back("==");
										this->i++;
									}
									else
										parametri.push_back(stringacheck);
								stringacheck = splitted[++this->i];
							} while (stringacheck != ")" || this->i >= splitted.size());

							if (nomefunzione == "print")
							{
								this->print(parametri);
							}
							else if (nomefunzione == "if")
							{
								cout << "if statement" << endl;
								const string if1 = parametri[0];
								const string paragone = parametri[1];
								const string if2 = parametri[2];


								this->If(if1, paragone, if2);
							}
							else
							{
								Function func = this->find_function(nomefunzione);
								if (func.get_name() == "")
								{
									try
									{
										if (splitted[i-3] == "func")
										{
											cout << "Aggiungo funzione" << endl;
											Function func;
											func.setup(nomefunzione);
											this->funzioni.push_back(func);
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
					else if (stringa == "{")
					{
						if (this->Ifs.size() > 0)
						{
							cout << "Inizio If" << endl;
							const int ultimoelem = Ifs.size() - 1;
							if (!this->Ifs[ultimoelem])
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
					else if (stringa == "}")
					{
						if (this->Ifs.size() > 0)
						{
							cout << "Chiusura if" << endl;
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
			lastStringa = splitted[i];
		}
		/*if (this->Ifs.size() == 0 && this->FindindGraffa)
		{
			cout << "Error: expected { after if statement" << endl;
			return;
		}*/
		//cout << lastStringa << endl;
	}
	catch (const exception& errore)
	{
		cout << errore.what() << endl;
	}
	//cout << endl;
}

Variable Interprete::find_variable(string name)
{
	bool trovata = false;
	for (int i = 0; i < this->variabili.size(); i++)
	{
		Variable* var = &this->variabili[i];
		if (var->get_name() == name)
		{
			trovata = true;
			//cout << "Trovata! " << var->get_name() << " = " << var->get_int_value() << endl;
			return *var;
		}
	}
	if (!trovata)
	{
		Variable var;
		//cout << var.get_str_value() << endl;
		return var;
	}
}

void Interprete::loadIntVariable(vector<string> splitted, string name)
{
	string typeValoreFinale;
	string valore = splitted[(++this->i)];
	Variable Var = this->find_variable(valore);
	const string type = Var.get_type();
	if (type != "")
	{
		//cout << "e' una variabile" << endl;
		if (type == "string")
			valore = Var.get_str_value();
		else if (type == "int")
			valore = to_string(Var.get_int_value());
	}
	this->AddInteri(splitted, &valore, &typeValoreFinale);
	if (typeValoreFinale == "invalid")
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
		bool trovata = false;
		for (int i = 0; i < this->variabili.size(); i++)
		{
			Variable* var = &this->variabili[i];
			if (var->get_name() == name)
			{
				var->set_value(stoi(valore));
				trovata = true;
				break;
			}
		}
		if (!trovata)
		{
			Variable var;
			var.setup(name, stoi(valore));
			if (this->Ifs.size() > 0)
			{
				int size = VariablesInIfs.size();
				if (VariablesInIfs.size() <= size)
				{
					vector<Variable> vettore;
					VariablesInIfs.push_back(vettore);
				}
				this->VariablesInIfs[size].push_back(var);
			}
			else
				this->variabili.push_back(var);
		}
	}

	//cout << "Output: " << var.get_int_value() << endl;
}

void Interprete::loadStringVariable(vector<string> splitted, string name)
{
	string typeValoreFinale;
	string valore = splitted[(++this->i)];
	
	this->AddStringhe(splitted, &valore, &typeValoreFinale);
	//cout << valore << " e' il valore della variabile" << endl;
	if (!isNan(string(1, (name[0]))))
	{
		cout << "Error: a variable's name cannot start with a number" << endl;
	}
	else
	{
		bool trovata = false;
		for (int i = 0; i < this->variabili.size(); i++)
		{
			Variable* var = &this->variabili[i];
			if (var->get_name() == name)
			{
				var->set_value(valore);
				trovata = true;
				break;
			}
		}
		if (!trovata)
		{
			Variable var;
			var.setup(name, valore);
			this->variabili.push_back(var);
		}
	}

	//cout << "Output: " << var.get_int_value() << endl;
}

void Interprete::printString(string stringa)
{
	for (int i = 0; i < stringa.length(); i++)
	{
		char carattere = stringa[i];
		if (carattere != '"')
		{
			if (carattere == '\\')
			{
				string parola = (string(1, carattere) + string(1, stringa[++i]));
				//cout << endl << "parola = " << parola << (parola == "\\n") << endl;
				if (parola == "\\n")
				{
					cout << endl;
				}
			}
			else
			{
				//cout << "carattere = " << carattere << " " << (carattere == ' ') << endl;
				if (carattere == ' ')
					cout << " " << string(1, stringa[++i]);
				else
					cout << carattere;
			}
		}
	}
}

void Interprete::AddInteri(std::vector<std::string> splitted, string* valore, string* typeValoreFinale)
{
	do
	{
		//cout << valore << endl;
		if ((this->i + 1) >= splitted.size() || splitted[this->i + 1] == "")
			break;
		if (splitted[this->i + 1] == "+")
		{
			const string addendo1 = *valore;
			string addendo2 = splitted[(this->i += 2)];

			Variable var = this->find_variable(addendo2);
			const string type = var.get_type();
			if (type != "")
			{
				//cout << "e' una variabile" << endl;
				if (type == "string")
					addendo2 = var.get_str_value();
				else if (type == "int")
					addendo2 = to_string(var.get_int_value());
			}
			//cout << addendo1 << " e' l'addendo 1" << endl;
			//cout << addendo2 << " e' l'addendo 2" << endl;
			bool isNan1 = isNan(addendo1);
			bool isNan2 = isNan(addendo2);

			//cout << isNan1 << " " << isNan2 << endl;

			if (isNan1 || isNan2)
			{
				cout << "Error: invalid type of value" << endl;
				*typeValoreFinale = "invalid";
				break;
				/*
				valore = (addendo1 + addendo2);
				typeValoreFinale = "string";
				cout << "output: " << valore << endl;
				*/
			}
			else
			{
				int risultato = stoi(addendo1) + stoi(addendo2);
				//cout << "risultato = " << risultato << endl;
				*valore = to_string(risultato);
				//cout << "addendo1 = " << addendo1 << endl;
				//cout << "addendo2 = " << addendo2 << endl;
				*typeValoreFinale = "int";
				//valore = (stoi(addendo1) + stoi(addendo2));
				//cout << "valore here: " << valore << endl;
				//cout << "output: " << valore << endl;
			}
		}
		//cout << "fine" << endl;
	} while (true);
}

void Interprete::AddStringhe(std::vector<std::string> splitted, string* valore, string* typeValoreFinale)
{
	do
	{
		if ((this->i + 1) >= splitted.size() || splitted[this->i + 1] == "")
			break;
		if (splitted[this->i + 1] == "+")
		{
			const string addendo1 = *valore;
			const string addendo2 = splitted[(this->i += 2)];
			string risultato = addendo1 + addendo2;
			string newRisultato;
			for (int i = 0; i < risultato.size(); i++)
			{
				if (i == 0 || i == risultato.size() - 1)
				{
					newRisultato += '"';
				}
				else
				{
					const char carattere = risultato[i];
					if (carattere != '"')
						newRisultato += carattere;
				}
			}
			//cout << "risultato = " << newRisultato << endl;
			*valore = newRisultato;
			//cout << "addendo1 = " << addendo1 << endl;
			//cout << "addendo2 = " << addendo2 << endl;
			*typeValoreFinale = "string";
			//cout << addendo1 << " e' l'addendo 1" << endl;
			//cout << addendo2 << " e' l'addendo 2" << endl;

			//cout << isNan1 << " " << isNan2 << endl;
		}
	} while (true);
}

void Interprete::If(const string if1, const string paragone, const string if2)
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

	if (paragone == "==")
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

void Interprete::FindGraffa(vector<string> splitted)
{
	bool trovata = false;
	int numgraffeaperte = 0;
	do
	{
		if (this->i >= splitted.size()-1)
		{
			break;
		}
		this->i++;
		//cout << splitted[this->i] << " " << " " << numgraffeaperte << endl;
		if (splitted[this->i] == "{")
			numgraffeaperte++;
		if (splitted[this->i] == "}")
			numgraffeaperte--;
	} while (splitted[this->i] != "}" && numgraffeaperte > 0);
	if (splitted[this->i] == "}" && numgraffeaperte == 0)
		trovata = true;
	//cout << "trovata = " << trovata << endl;

	this->FindindGraffa = !trovata;

	if (trovata)
	{
		this->Ifs.erase(this->Ifs.end() - 1);
		//this->FindindGraffa = false;
	}
	/*for (int i = 0; i < this->Ifs.size(); i++)
	{
		cout << "Ifs " << i << " " << Ifs[i] << endl;
	}*/
}