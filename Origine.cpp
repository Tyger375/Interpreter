#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "./Interpreter/Interpreter.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        Interpreter interprete;
        interprete.start(argv[1]);
    }
    else
    {
        cout << "Real-time interpreter is not available. It'll be added soon" << endl;
    }
    return 0;
}