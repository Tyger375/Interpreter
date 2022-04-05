#ifndef INTERPRETER_WHILE_H
#define INTERPRETER_WHILE_H

#include <string>
#include <iostream>
#include <vector>
#include "../Variable/Variable.h"

class While
{
private:
    std::vector<std::vector<std::string>> lines;
    std::vector<std::string> WhileCondition;
public:
    void add_line(std::vector<std::string>);
    void add_condition(std::vector<std::string>);

    inline std::vector<std::vector<std::string>> get_lines() { return this->lines; };
    void execute(std::vector<Variable>);
};

#endif //INTERPRETER_WHILE_H
