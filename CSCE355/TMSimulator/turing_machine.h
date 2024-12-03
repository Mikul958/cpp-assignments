// Copyright 2024 Michael Pikula

#ifndef _TURINGMACHINE_H_
#define _TURINGMACHINE_H_

#include "tape.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>          // Using ifstream to read files

using std::string;
using std::vector;
using std::unordered_map;

// Transition associated with a specific state and input
struct Transition {
    char write;       // Symbol to write before head moves
    bool headRight;   // Direction for head to move; false for left, true for right
    int next;         // Index of the State this transition will go to.
};

// State in a Turing machine, containing a name and set of transitions
struct State {
    string name;                                  // Name of state as given in text file
    unordered_map<char, Transition> transitions;  // Hash table mapping a character input to its transition
};

class TuringMachine
{
    public:
        bool Initialize(string);
        bool ReadInputs(string);
        
        bool Run();

    private:
        vector<State> states;
        Tape tape;

        bool isTransducer;
        vector<string> inputs;   // List of whole input strings
        vector<string> results;  // List of tests on each input string
};

#endif  // _TURINGMACHINE_H_