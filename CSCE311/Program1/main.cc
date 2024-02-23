// Copyright 2024 mpikula

#include <Program1/calculate.h>

#include <iostream>
using std::cout;
using std::endl;
#include <stdexcept>
using std::invalid_argument;

int main(int argc, char* argv[]) {
    try {
        cout << Calculate(argc, argv) << endl;
    }
    catch(const invalid_argument& e) {
        cout << e.what() << endl;
    }
    return 0;
}
