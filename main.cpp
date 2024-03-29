#include <iostream>
#include <string>
#include <vector>
#include "./Interpreter/Interpreter.h"
#include "./Other/Utilities/Utilities.h"

using namespace std;
using namespace Utilities;

int main(int argc, char** argv)
{
    interpreter::Interpreter interpreter;
    bool debug = false;
    bool savelogs = false;
    string logs_file_name = "logs.txt";
    if (argc >= 2)
    {
        vector<string> words;
        for (int i = 1; i < argc; ++i)
        {
            string word(argv[i]);
            words.push_back(word);
        }

        for (auto word : words)
        {
            if (word.length() > 2)
            {
                if (word[0] == '-' && word[1] == '-')
                {
                    string newWord;
                    for (int j = 2; j < word.size(); ++j)
                    {
                        newWord += word[j];
                    }
                    vector<string> splitted = split(newWord, '=');
                    if (splitted.size() >= 2)
                    {
                        if (splitted[0] == "debug")
                        {
                            if (splitted[1] == "true")
                                debug = true;
                            else if (splitted[1] == "false")
                                debug = false;
                        }
                        else if (splitted[0] == "savelogs")
                        {
                            if (splitted[1] == "true")
                                savelogs = true;
                            else if (splitted[1] == "false")
                                savelogs = false;
                        }
                    }
                }
            }
        }
    }
    if (argv[1][0] != '-')
    {
        interpreter.start(argv[1], debug, savelogs, logs_file_name);
    }
    else
    {
        string line;
        do
        {
            cout << ">>> ";
            getline(cin, line);
            interpreter.Line(line);
            //interpreter.debugVariables();
            cout << '\b';
        }
        while (!interpreter.is_error());
        if (debug)
        {
            interpreter.debugVariables(savelogs);
            interpreter.debugFunctions(savelogs);
            interpreter.debugInternals(savelogs);
        }
        if (savelogs)
        {
            interpreter.saveLogs();
        }
        //interpreter(savelogs);
        //cout << "Real-time interpreter is not available. It'll be added soon" << endl;
    }
    return 0;
}