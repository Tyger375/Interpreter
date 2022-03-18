#include "Utilities.h"
#include <iostream>
#include <vector>
#include <string>
#include "Variable.h"

using namespace std;

string getValueFromString(string stringa)
{
	string newStringa;
	for (int i = 0; i < stringa.length(); i++)
	{
		char carattere = stringa[i];
		if (carattere != '"')
		{
			newStringa += carattere;
		}
	}
	return newStringa;
}