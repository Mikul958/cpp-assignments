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


/**
 * Transition associated with a specific state and input
 * @param nextState String; name of the state this transition leads to
 * @param write Char; Symbol to write on tape at head before it moves
 * @param goRight Bool; direction for head to move - true for right, false for left
 */
struct Transition
{
    string nextState;
    char write;
    bool goRight;
};

/**
 * State of a turing machine
 * @param name String; name of this state
 * @param isFinal Bool; true if this state is accepting
 * @param transitions unordered_map<char, Transition>; hash table mapping a character input to a transition
 */
struct State
{
    string name;
    bool isFinal = false;
    unordered_map<char, struct Transition> transitions;
};

/**
 * Class representing a Turing Machine; simulates Turing Machine tests given info and input files
 * @author Michael Pikula
 */
class TuringMachine
{
    public:
        
        // Initialize and run
        explicit TuringMachine(string, string);
        bool run();

        // Get simulation results
        vector<string> getInputs();
        vector<string> getResults();
        string getError();

    private:
        
        // TM properties, loaded from file and persist
        bool isTransducer;
        string initialState;
        string sigma;                              // String containing input alphabet, used to ensure input strings are valid
        string gamma;                              // String containing tape alphabet, used to ensure all transition inputs are valid
        unordered_map<string, State> stateList;    // Hash table mapping a state's name to itself
        Tape tape;

        // Input strings and corresponding esults; have accessors
        vector<string> inputs;
        vector<string> results;

        // Used for file reading and error detection
        std::ifstream currentFile;  // Current open file
        string fileLine;            // Last-read file line
        int lineNumber = -1;        // Line number of last-read file line
        bool isOK = false;          // Has Turing Machine successfully initialized?
        string error;               // Contains Turing Machine error, if applicable


        // Helper functions, grouped by purpose
        bool loadTuringMachine(string);
        bool loadInputs(string);
        
        void addResultRecognizer(bool);
        void addResultTransducer();

        bool openFile(string);
        void closeFile();
        bool getFileLine();
        vector<string> split(string, char);
};

#endif  // _TURINGMACHINE_H_