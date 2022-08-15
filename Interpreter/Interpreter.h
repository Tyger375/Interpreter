#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <iostream>
#include <vector>
#include "../Function/Function.h"
#include "../While/While.h"

using std::vector;
using std::string;

namespace interpreter
{
    class Interpreter;
}

class interpreter::Interpreter
{
private:
    //Interpreter
    string logs_file_name;
    string all_data_logs;
    int i{};
    bool error{};
    bool writingComment{};

    //Variables
    vector<string> typeVariables;
    string mainWriting;

    //Lists Variables
    vector<string> listAll;
    vector<bool> writingList;
    vector<Variable> ListWriting;

    //Dictionaries Variables
    vector<string> Keys;
    bool writingKey{};
    vector<string> DictAll;
    vector<bool> writingDict;
    vector<Variable> DictWriting;

    //Functions
    vector<Function> functions;
    bool writingFunc{};
    Function* FUNC{};
    bool isExecutingFunc{};
    //Whiles
    vector<While> whiles;
    vector<bool> writingWhile;
    //Ifs
    vector<bool> Ifs;
    bool FindingElse{};
    int FindingFromLine{};

    void Setup();

    void SetReturnValue(vector<string>);
    void executeCustomFunction(Function*, vector<string>);
    void loadVariable(vector<string>, const string&);
    Variable loadVariableWithoutWriting(vector<string>, const string&);
    static void printString(string);
    void Operation(vector<string>, string*, string*, const string&, int*);

    //List
    static void printList(Variable);
    void loadList(vector<string>, bool, int*);

    //Dict
    void LoadDictItem(bool);
    void printDict(Variable);
    void loadDict(vector<string>, bool, int*);

    //Operations
    bool Addition(string*, string*, vector<string>, string, int*);
    bool Subtraction(string*, string*, vector<string>, string, int*);

    //Deprecated
    void AddIntegers(vector<string>, string*, string*);
    void AddStrings(vector<string>, string*, string*);

    //Functions
    void print(const vector<string>&, bool);
    Variable cast_int(const vector<string>& parameters);
    Variable cast_string(const vector<string>& parameters);
    Variable cast_bool(const vector<string>& parameters);
    Variable cast_list(const vector<string>& parameters);
    string Typeof(vector<string>, bool*);
    void If(const vector<string>&);
    //for (i=0;i>0;i++)
    void ForLoop(const vector<string>&, const vector<string>&, const vector<string>&);
    void WhileLoop(const vector<string>&);
    void WriteParameters(vector<string>, vector<string>*, bool, bool, int*);
    void LoadParamVariable(vector<string>*, bool, vector<string>*);
    void InternalFunctionLoadVariable(string*, string*, vector<string>, string, int*);

    Variable execute_internal_function(Variable*, const string&, vector<string>);

    //Internal functions
    //Lists
    Variable internal_add(Variable*, vector<string>);
    Variable internal_remove(Variable*, vector<string>);
    Variable internal_contains(Variable*, vector<string>);
    //Strings
    Variable internal_split(Variable*, vector<string>);
    static Variable internal_lower(Variable*);
    static Variable internal_upper(Variable*);
    //Dicts
    Variable internal_keys(Variable*);
    //Multiple types
    Variable internal_length(Variable*);
public:
    int line{};

    Interpreter(const vector<Variable>&, const vector<Variable>&, const vector<Function>&, bool, Function*);
    Interpreter();
    inline const bool is_error() const { return this->error; }
    void clear();
    void start(const string&, bool, bool, string);
    void Line(string str_line);
    void debugVariables(bool);
    void debugFunctions(bool);
    void debugInternals(bool);
    void saveLogs();
    void PrintError(const string&);

    inline vector<Variable> getVariables() const { return this->variables; };

    Variable find_variable(const string&);
    Variable* find_variable_pointer(const string&);
    Function find_function(const string&);
    vector<Variable> variables;

    Variable executeFunction(const string& , bool, const vector<string>&, bool, bool*);

    vector<vector<Variable>> VariablesInfos;
};

#endif //INTERPRETER_INTERPRETER_H
