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
    string next;      // Name of the State this transition will go to.
    char write;       // Symbol to write before head moves
    bool goRight;     // Direction for head to move; false for left, true for right
};

// State in a Turing machine, containing a name and set of transitions
struct State {
    string name;                                  // Name of state as given in text file
    bool isFinal = false;
    unordered_map<char, Transition> transitions;  // Hash table mapping a character input to its transition
};

class TuringMachine
{
    public:
        bool loadTM(string);
        bool loadInputs(string);
        
        bool run();

        void cleanLine(string*);
        vector<string> split(string, char);

    private:
        unordered_map<string, State> stateList;  // Hash table mapping a state's name to itself
        string currentState;                     // Name of the current state
        Tape tape;                               // Tape that the TM will use

        bool isTransducer;
        vector<char> sigma;       // Input alphabet, used to ensure input strings are valid
        vector<char> gamma;       // Tape alphabet, used to ensure all transition inputs are valid

        vector<string> inputs;   // List of whole input strings
        vector<string> results;  // List of tests on each input string
};

#endif  // _TURINGMACHINE_H_