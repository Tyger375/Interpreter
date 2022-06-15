#include <iostream>
#include <fstream>
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
    if (argc >= 2)
    {
        vector<string> words;
        for (int i = 1; i < argc; ++i)
        {
            string word(argv[i]);
            words.push_back(word);
        }

        for (int i = 0; i < words.size(); ++i)
        {
            string word = words[i];
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
                    }
                }
            }
        }
        interpreter.start(argv[1], debug);
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
        }
        while (line != "exit");
        interpreter.debugVariables();
        //cout << "Real-time interpreter is not available. It'll be added soon" << endl;
    }
    return 0;
}