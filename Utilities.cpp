#include "Utilities.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> Utilities::split(string String, char splitter)
{
	vector<string> operatori = {
		"+",
		"-",
		"/",
		"*",
		"=",
		"(",
		")",
		",",
		"{",
		"}"
	};
	string word = "";
	vector<string> words;
	bool isStringa = false;
	try
	{
		for (int i = 0; i < (String.length()); i++)
		{
			char pog = String[i];
			//cout << pog << endl;
			//cout << i << stringa.length() << endl;
			if (((pog == splitter && !isStringa) || i == String.length() - 1))
			{
				//cout << word << endl;
				if (pog == splitter && word != "" && !isStringa) {
					words.push_back(word);
					word = "";
				}
				else if (i == String.length() - 1)
				{
					if (findInVector(operatori, string(1, pog)))
					{
						if (word != "")
							words.push_back(word);
						words.push_back(string(1, pog));
						word = "";
					}
					else
					{
						word += pog;
						words.push_back(word);
						word = "";
					}
				}
			}
			else
			{
				string chartostring = string(1, pog);
				if (pog == '"')
				{
					isStringa = !isStringa;
					word += pog;
				}
				else if (findInVector(operatori, chartostring) && !isStringa)
				{
					//cout << word << " " << pog << endl;
					if (word != "")
						words.push_back(word);
					words.push_back(string(1, pog));
					word = "";
				}
				else
				{
					//cout << pog << endl;
					if (pog != '\t')
						word += pog;
				}
			}
			//cout << word << endl;
		}
	}
	catch (const exception& test)
	{
		cout << test.what() << endl;
	}
	/*for (int i = 0; i < (words.size() - 1); i++)
	{
		cout << &words[i] << endl;
	}*/
	return words;
}

vector<char> Utilities::searchOperatori(string String)
{
	vector<string> operatori = {
		"+",
		"-",
		"/",
		"*",
		"="
	};
	string Parola = "";
	vector<string> Parole;
	vector<char> Operatori;
	for (int i = 0; i < String.length(); i++)
	{
		const char carattere = String[i];
		const string charToString = string(1, carattere);
		//cout << charToString << endl;
		bool FoundInVector = findInVector(operatori, charToString);
		if (FoundInVector || i == String.length() - 1)
		{
			if (i == String.length() - 1)
			{
				Parola += charToString;
			}
			if (FoundInVector)
			{
				Operatori.push_back(carattere);
			}
			//cout << Parola << endl;
			Parole.push_back(Parola);
			Parola = "";
		}
		else
		{
			Parola += charToString;
		}
	}
	return Operatori;
}

vector<string> Utilities::OperatoriParole(string String)
{
	vector<string> operatori = {
		"+",
		"-",
		"/",
		"*",
		"="
	};
	string Parola = "";
	vector<string> Parole;
	vector<char> Operatori;
	for (int i = 0; i < String.length(); i++)
	{
		const char carattere = String[i];
		const string charToString = string(1, carattere);
		//cout << charToString << endl;
		bool FoundInVector = findInVector(operatori, charToString);
		if (FoundInVector || i == String.length() - 1)
		{
			if (i == String.length() - 1 && !FoundInVector && charToString != "")
			{
				Parola += charToString;
			}
			//cout << Parola << endl;
			Parole.push_back(Parola);
			Parola = "";
		}
		else
		{
			Parola += charToString;
		}
	}
	return Parole;
}

bool Utilities::findInVector(vector<string> list, string key)
{
	if (count(list.begin(), list.end(), key)) {
		return true;
	}
	else {
		return false;
	}
}

bool Utilities::isNan(string String)
{
	try
	{
		int intero = stoi(String);
		return false;
	}
	catch (const exception&)
	{
		return true;
	}
}