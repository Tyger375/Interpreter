#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <iostream>
#include <vector>
#include "../Function/Function.h"
#include "../While/While.h"

using namespace std;

namespace interpreter
{
    class Interpreter;
}

class interpreter::Interpreter
{
private:
    int i;
    std::vector<std::string> typeVariables;
    std::vector<Function> functions;
    std::vector<While> whiles;
    std::vector<bool> Ifs;
    std::vector<std::vector<Variable>> VariablesInfos;
    bool FindindStaple;
    bool writingFunc;
    bool writingWhile;
    Function* FUNC;

    bool isExecutingFunc;

    void Setup();

    void SetReturnValue(vector<string>);
public:
    Interpreter(vector<Variable>, bool, Function*);
    Interpreter();
    void start(std::string);
    void Line(std::string line);
    void debugVariables();
    void debugFunctions();

    inline vector<Variable> getVariables() { return this->variables; };

    Variable find_variable(std::string);
    Variable* find_variable_pointer(std::string);
    Function find_function(std::string);
    //void addizione();
    void loadVariable(std::vector<std::string>, std::string);
    void loadIntVariable(std::vector<std::string>, std::string);
    void loadStringVariable(std::vector<std::string>, std::string);
    void printString(std::string);
    void Add(std::vector<std::string>, std::string*, std::string*, std::string);
    void AddIntegers(std::vector<std::string>, std::string*, std::string*);
    void AddStrings(std::vector<std::string>, std::string*, std::string*);
    void print(std::vector<std::string>);
    void If(const std::string, const std::string, const std::string);
    //for (i=0;i>0;i++)
    void ForLoop(const std::vector<std::string>, const std::vector<std::string>, const std::vector<std::string>);
    void WhileLoop(const std::vector<std::string>);
    void FindGraffa(std::vector<std::string>);

    std::vector<Variable> variables;
};

#endif //INTERPRETER_INTERPRETER_H
