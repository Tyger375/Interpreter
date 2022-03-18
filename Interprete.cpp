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
			const string stringa = splitted[i];
			//cout << stringa << endl;
			//vector<char> Operatori = searchOperatori(stringa);
			//vector<string> Parole = OperatoriParole(stringa);
			bool FoundInVector = findInVector(tipiVariabili, lastStringa);
			if (findInVector(tipiVariabili, stringa))
			{
				//cout << stringa << " e' un tipo variabile" << endl;
				//const string type = stringa;
				//const string nome = splitted[(++i)];
				//const string nome = splitted[(++i)];
			}
			else
			{
				if (FoundInVector)
				{
					//cout << stringa << " e' il nome di una variabile" << endl;
				}
				else
				{
					if (stringa == "=" && !FoundInVector)
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
					else if (stringa == "+" && !FoundInVector)
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
					else if (stringa == "(" && !FoundInVector)
					{
						if (lastStringa != "")
						{
							const string nomefunzione = lastStringa;
							//cout << "nome funzione = " << nomefunzione << endl;
							vector<string> parametri;
							string stringacheck = splitted[i];
							do
							{
								if (stringacheck != "," && stringacheck != "(")
									parametri.push_back(stringacheck);
								stringacheck = splitted[++this->i];
							} while (stringacheck != ")" || this->i >= splitted.size());
							if (nomefunzione == "print")
							{
								if (parametri.size() == 0)
									cout << "Error: no param given" << endl;
								else
								{
									const string primoparametro = parametri[0];
									if (primoparametro[0] == '"')
									{
										//cout << primoparametro << " e' una stringa" << endl;
										cout << "Output: ";
										this->printString(primoparametro);
									}
									else
									{
										//cout << primoparametro << " e' una variabile" << endl;
										Variable var = this->find_variable(primoparametro);
										const string type = var.get_type();
										if (type == "string")
										{
											cout << "Output: ";
											this->printString(var.get_str_value());
											cout << endl;
											//cout << "Output: " << var.get_str_value() << endl;
										}
										else if (type == "int")
										{
											cout << "Output: " << var.get_int_value() << endl;
										}
									}
								}
							}
						}
						else
						{
							cout << "non e' una funzione" << endl;
						}
					}
				}
			}
			//cout << endl;
			lastStringa = splitted[i];
		}
		//cout << lastStringa << endl;
	}
	catch (const exception& errore)
	{
		cout << errore.what() << endl;
	}
	//cout << endl;
}

void Interprete::debugVariables()
{
	for (int i = 0; i < this->variabili.size(); i++)
	{
		Variable var = variabili[i];
		const string type = var.get_type();
		cout << var.get_name();
		if (type == "string")
		{
			cout << " " << var.get_str_value() << endl;
		}
		else if (type == "int")
		{
			cout << " " << var.get_int_value() << endl;
		}
	}
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