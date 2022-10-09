#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int main()
{
    ofstream myfile;
    string filename = "SMA.csv";
    myfile.open(filename);
    myfile << "Hi"
           << ","
           << "SMA" << endl;
    return 0;
}