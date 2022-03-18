#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Utilities.h"
#include "Variable.h"
#include "Interprete.h"

using namespace Utilities;
using namespace std;

int main() {
    fstream newfile;
    //cout << " " << "ciao" << " " << "due" << endl;
    //cout << *(ciao.get_value()) << endl;
    //cout << *(ciao2.get_value()) << endl;
	//Int ciao("buongiorno", 2);
	//cout << ciao.name << endl;
    newfile.open("test.lmao", ios::in); //open a file to perform read operation using file object
    Interprete interprete;
    if (newfile.is_open()) //checking whether the file is open
    {
        string tp;
        while (getline(newfile, tp)) //read data from file object and put it into string.
        {
            //cout << tp << endl;   //print the data of the string
            interprete.Linea(tp);
        }
        interprete.debugVariables();
        interprete.debugFunctions();
        newfile.close();   //close the file object.
    }
    else
    {
        cout << "F" << endl;
    }
}