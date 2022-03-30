#include "./Utilities/Utilities.h"
#include <iostream>
#include <vector>
#include <string>
#include "../Variable/Variable.h"

using namespace std;

string getValueFromString(string String)
{
	string newString;
	for (int i = 0; i < String.length(); i++)
	{
		char carattere = String[i];
		if (carattere != '"')
		{
			newString += carattere;
		}
	}
	return newString;
}