#ifndef INTERPRETER_WHILE_H
#define INTERPRETER_WHILE_H

#include <string>
#include <iostream>
#include <vector>

class While
{
private:
    std::vector<std::vector<std::string>> lines;
public:
    void add_line(std::vector<std::string>);
    inline std::vector<std::vector<std::string>> get_lines() { return this->lines; };
};

#endif //INTERPRETER_WHILE_H
