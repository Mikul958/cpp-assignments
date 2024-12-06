// Copyright 2024 Michael Pikula

#include "turing_machine.h"

#include <iostream>
using std::cout;
using std::endl;

// Main method showcasing the functionality of the TuringMachine class
// Initializes and runs Turing Machine simulator, then prints results or errors
int main(int argc, char* argv[])
{
    // Validate usage
    if (argc != 3) {
        cout << "\n\tusage: ./tm_simulator <turing machine file> <input file>\n" << endl;
        return 1;
    }
    
    // Initialize TuringMachine instance using tm and input files and attempt to run
    TuringMachine simulator(argv[1], argv[2]);
    if (!simulator.run()) {
        cout << "\n\tFAILED TO INITIALIZE TURING MACHINE:\n\t" << simulator.getError() << "\n" << endl;
        return 2;
    }

    // Print results
    for (size_t i=0; i < simulator.getResults().size(); i++)
        cout << simulator.getInputs()[i] << " - " << simulator.getResults()[i] << endl;

    return 0;
}
