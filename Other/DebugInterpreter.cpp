#include "../Interpreter/Interpreter.h"
#include "./Utilities/Utilities.h"
#include "../Variable/Variable.h"
#include <iostream>

using namespace std;
using namespace Utilities;
using namespace interpreter;

void Interpreter::debugVariables(bool save)
{
    string data = "-----------------\n";
    data += string("Debugging variables:") + "\n";
	for (int j = 0; j < this->variables.size(); j++)
	{
		Variable var = variables[j];
		data += var.get_name() + " " + var.get_value() + "\n";
	}

    data += string("Others: ") + "\n";

    for (int k = 0; k < this->VariablesInfos.size(); k++)
    {
        vector<Variable> vars = VariablesInfos[k];
        for (int j = 0; j < vars.size(); ++j)
        {
            Variable var = VariablesInfos[k][j];
            data += var.get_name() + " " + var.get_value() + "\n";
        }
    }
    cout << data;
    if (save)
    {
        all_data_logs += data;
    }
}

void Interpreter::debugFunctions(bool save)
{
    string data = "-----------------\n";
    data += string("Debugging functions:") + "\n";
	for (int i = 0; i < this->functions.size(); i++)
	{
		Function func = this->functions[i];
		data += func.get_name() + "\n";
	}
    cout << data;
    if (save)
    {
        all_data_logs += data;
    }
}

void Interpreter::debugInternals(bool save)
{
    string data = "-----------------\n";
    data += string("Debugging internal variables:") + "\n";
    data += "isExecutingFunc: " + to_string(this->isExecutingFunc) + "\n";
    data += "writingFunc: " + to_string(this->writingFunc) + "\n";
    data += "line: " + to_string(this->line) + "\n";
    data += "FindingElse: " + to_string(this->FindingElse) + "\n";
    data += "writingWhile.size(): " + to_string(this->writingWhile.size()) + "\n";
    data += "whiles.size(): " + to_string(this->whiles.size()) + "\n";
    data += "Ifs.size(): " + to_string(this->Ifs.size()) + "\n";
    data += "functions.size(): " + to_string(this->functions.size()) + "\n";
    data += "variables.size(): " + to_string(this->variables.size()) + "\n";
    data += "FindingFromLine: " + to_string(this->FindingFromLine) + "\n";
    data += "writingList.size(): " + to_string(this->writingList.size()) + "\n";
    data += "ListWriting.size(): " + to_string(this->ListWriting.size()) + "\n";
    data += "listAll.size(): " + to_string(this->listAll.size()) + "\n";
    data += "writingComment: " + to_string(this->writingComment) + "\n";
    data += "error: " + to_string(this->error) + "\n";
    data += "typeVariables.size(): " + to_string(this->typeVariables.size()) + "\n";
    cout << data;
    if (save)
    {
        all_data_logs += data;
    }
}

void Interpreter::saveLogs()
{
    ofstream local_logs_file(logs_file_name);

    local_logs_file << all_data_logs;
    local_logs_file.close();
}