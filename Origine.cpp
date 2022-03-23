#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Utilities.h"
#include "Variable.h"
#include "Interpreter.h"

using namespace Utilities;
using namespace std;

int main() {
    Interpreter interprete;
    interprete.start("test.lmao");
}