// Copyright 2024 mpikula

#include <Program1/calculate.h>

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <stdexcept>
using std::invalid_argument;

int main(int argc, char* argv[]) {
    vector<string> input;
    for (int i=1; i < argc; i++)
        input.push_back(string(argv[i]));
    
    try {
        cout << Calculate(input) << endl;
    }
    catch(const invalid_argument& e) {
        cout << e.what() << endl;
    }
    return 0;
}
