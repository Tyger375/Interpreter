#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "./Interpreter/Interpreter1.h"

using namespace std;

int main(int argc, char** argv)
{
    // Cool comment (viva python)
    Interpreter interpreter;
    if (argc == 2)
    {
        interpreter.start(argv[1]);
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
