// Copyright 2024 mpikula

#include <proj1/calculate.h>

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
    vector<string> input;
    for (int i=1; i < argc; i++)
        input.push_back(argv[i]);

    cout << Calculate(input) << endl;
}