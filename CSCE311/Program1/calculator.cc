#include <Program1/calculator.h>

// Temporary
#include <iostream>

#include <string>
using std::string;
#include <vector>
using std::vector;

void calculate(int argc, char* argv[])
{
    vector<string> input;
    for (int i=1; i<int(argc); i++)
        input.push_back(argv[i]);

    for (int i=0; i<input.size(); i++)
        std::cout << input[i] << "\n";
    std::cout << std::endl;
}