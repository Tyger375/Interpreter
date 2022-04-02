#include "../Other/Utilities/Utilities.h"
#include <iostream>
#include <vector>
#include <string>
#include "While.h"

using namespace std;
using namespace Utilities;

void While::add_line(vector<string> line)
{
    //cout << line.size() << endl;
    // Ao er contributore
    this->lines.push_back(line);
}