#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <iostream>
#include <vector>
#include "../Function/Function.h"
#include "../While/While.h"

namespace interpreter
{
    class Interpreter;
}

class interpreter::Interpreter
{
private:
    int i;
    int line;
    std::vector<std::string> typeVariables;
    std::vector<Function> functions;
    std::vector<While> whiles;
    std::vector<bool> Ifs;
    std::vector<std::vector<Variable>> VariablesInfos;
    std::vector<std::string> listAll;
    bool error;
    bool FindindStaple;
    bool writingFunc;
    bool writingWhile;
    std::vector<bool> writingList;

    std::vector<Variable> ListWriting;

    bool FindingElse;
    int FindingFromLine;

    Function* FUNC;

    bool isExecutingFunc;

    void Setup();

    void SetReturnValue(std::vector<std::string>);
    void executeCustomFunction(Function*, std::vector<std::string>);
    void loadVariable(std::vector<std::string>, const std::string&);
    Variable loadVariableWithoutWriting(std::vector<std::string>, std::string);
    void printString(std::string);
    void Operation(std::vector<std::string>, std::string*, std::string*, const std::string&, int*);

    //List
    void printList(Variable);
    void loadList(std::vector<std::string>, bool, int*);

    //Operations
    bool Addition(std::string*, std::string*, std::vector<std::string>, std::string, int*);
    bool Subtraction(std::string*, std::string*, std::vector<std::string>, std::string, int*);

    //Deprecated
    void AddIntegers(std::vector<std::string>, std::string*, std::string*);
    void AddStrings(std::vector<std::string>, std::string*, std::string*);
    void loadIntVariable(std::vector<std::string>, std::string);
    void loadStringVariable(std::vector<std::string>, std::string);
    //Functions
    void print(std::vector<std::string>);
    std::string Typeof(std::vector<std::string>, bool*);
    void If(std::vector<std::string>); //std::string, std::string, std::string
    //for (i=0;i>0;i++)
    void ForLoop(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>);
    void WhileLoop(std::vector<std::string>);
    void FindGraffa(std::vector<std::string>);
    void WriteParameters(std::vector<std::string>, std::vector<std::string>*, bool, bool, int*);

    void execute_internal_function(Variable*, const std::string&, std::vector<std::string>, bool*);

    //Internal functions
    //Lists
    void internal_add(Variable*, std::vector<std::string>, bool*);
public:

    Interpreter(std::vector<Variable>, bool, Function*);
    Interpreter();
    void start(std::string, bool);
    void Line(std::string line);
    void debugVariables();
    void debugFunctions();
    void PrintError(std::string);

    inline std::vector<Variable> getVariables() const { return this->variables; };

    Variable find_variable(std::string);
    Variable* find_variable_pointer(std::string);
    Function find_function(std::string);

    std::vector<Variable> variables;
    Variable executeFunction(const std::string& , bool, std::vector<std::string>, bool, bool*);
};

#endif //INTERPRETER_INTERPRETER_H
