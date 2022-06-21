#ifndef INTERPRETER_WHILE_H
#define INTERPRETER_WHILE_H

#include <string>
#include <iostream>
#include <vector>
#include "../Variable/Variable.h"
#include "../Function/Function.h"

using std::vector;
using std::string;

class While
{
private:
    int Opened = 0;
    int Closed = 0;
    vector<vector<string>> lines;
public:
    void add_line(const vector<string>&);
    void add_condition(vector<string>);
    void execute(const vector<Variable>&, const vector<Function>&);

    vector<string> WhileCondition;
};

#endif //INTERPRETER_WHILE_H
