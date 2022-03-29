#include "../Other/Utilities/Utilities.h"
#include <iostream>
#include <vector>
#include <string>
#include "Variable.h"

using namespace std;
using namespace Utilities;

void Variable::setup(string nome, int val)
{
	this->IntValue = val;
	this->name = nome;
	this->valueType = "int";
}

void Variable::setup(string nome, string val)
{
	this->StrValue = val;
	this->name = nome;
	this->valueType = "string";
}

void Variable::set_value(int newval)
{
	this->IntValue = newval;
}

void Variable::set_value(string newval)
{
	this->StrValue = newval;
}

int Variable::get_int_value()
{
	return this->IntValue;
}

string Variable::get_str_value()
{
	return this->StrValue;
}