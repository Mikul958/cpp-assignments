// Copyright 2024 Michael Pikula

#include "turing_machine.h"

#include <iostream>
using std::cout;
using std::endl;


/**
 * Initializes states, alphabet, and blank symbol of Turing Machine using given TM text file
 * @param filename Path of a text file containing Turing Machine info (Ex. data/wwr_tm.txt)
 * @return true if load successful, false otherwise
 */
bool TuringMachine::loadTuringMachine(string filename)
{
    // Open file using ifstream
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        this->error = "path \"" + filename + "\' not found";
        return false;
    }

    // Read lines until all comments are skipped
    string currentLine;
    while (std::getline(file, currentLine)) {
        if (currentLine.substr(0, 2) != "//")
            break;
    }

    // LINE 0: Check first uncommented line reads "TM"
    cleanLine(&currentLine);
    if (currentLine != "TM")
        return false;

    // LINE 1: Add empty states to TM hash map
    std::getline(file, currentLine);
    cleanLine(&currentLine);

    vector<string> tokens = split(currentLine, ',');
    for (string name : tokens) {
        struct State newState = {name};     // Create new State with name of current string
        this->stateList.insert({name, newState});
    }

    // LINE 2: Read in input alphabet (sigma)
    std::getline(file, currentLine);
    cleanLine(&currentLine);

    tokens = split(currentLine, ',');
    for (string symbol : tokens)
        this->sigma.push_back(symbol.front());

    // LINE 3: Read in tape alphabet (gamma)
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    
    tokens = split(currentLine, ',');
    for (string symbol : tokens)
        this->gamma.push_back(symbol.front());

    // LINE 4: Set initial state
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    this->initialState = currentLine;

    // LINE 5: Set blank symbol
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    this->tape.setBlank(currentLine.front());

    // LINE 6: Get list of final states and set all existing states as such
    std::getline(file, currentLine);
    cleanLine(&currentLine);

    vector<string> finalStates = split(currentLine, ',');
    for (string name : finalStates)
        this->stateList[name].isFinal = true;

    // LINE 7+: Read in deltas and add transitions to states; delta = (currentState, input, nextState, toWrite, direction)
    while (std::getline(file, currentLine)) {
        cleanLine(&currentLine);
        vector<string> delta = split(currentLine, ',');
        if (delta.size() != 5) {
            this->error = "Found delta of size " + std::to_string(delta.size()) + ", expected 5";  // TODO get line number
            return false;
        }

        // Retrieve information as correct data types from split delta
        string currentName = delta[0];
        char input = delta[1].front();
        string nextName = delta[2];
        char toWrite = delta[3].front();
        bool headRight = (delta[4] == "R");

        // Create new transition and add at specified state + input
        struct Transition newTransition = {nextName, toWrite, headRight};
        this->stateList[currentName].transitions.insert({input, newTransition});

        // TODO return false if input is not part of tape alphabet
    }

    return true;
}

/**
 * Loads test type and all input strings from the given input text file
 * @param filename Path of a text file containing input info (Ex. data/wwr_input.txt)
 * @return true if load successful, false otherwise
 */
bool TuringMachine::loadInputs(string filename)
{
    // Open file using ifstream
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        this->error = "path \"" + filename + "\' not found";
        return false;
    }

    // Get and clean first line of file
    string currentLine;
    std::getline(file, currentLine);
    cleanLine(&currentLine);

    // Check first line to ensure file is an input file and check simulation type
    if (currentLine == "Recognizer") {
        this->isTransducer = false;
    }
    else if (currentLine == "Transducer") {
        this->isTransducer = true;
    }
    else {
        this->error = "test type not recognized (expected \"Recognizer\" or \"Transducer\", actual \"" + currentLine + "\")";
        return false;
    }

    // Load inputs and close file
    while (std::getline(file, currentLine)) {
        cleanLine(&currentLine);
        this->inputs.push_back(currentLine);

        // Abort if input string contains a character not in input alphabet
        for (char input : currentLine) {
            if (sigma.find(input) == string::npos) {
                this->error = "found input character \'" + string(1, input) + "\' not in input alphabet specified by Turing Machine";
                return false;
            }
        }
    }
    file.close();
    return true;
}

/**
 * Simulates a Turing Machine test for all input strings
 * @return true if tests finish normally, false otherwise
 */
bool TuringMachine::run()
{
    for (string inputString : this->inputs)
    {
        // Reset all ephemeral properties
        this->currentState = initialState;
        this->tape.resetTape(inputString);   // Reset tape and load current input string onto it
        this->isAccepting = false;

        // Process tape inputs until halt accept or reject
        while (true)
        {
            // Test to see if current state is final, halt accept if yes
            if (this->stateList[this->currentState].isFinal) {
                this->isAccepting = true;
                break;
            }
            
            // Get current state from hash map using its name, then get its transition table
            unordered_map<char, struct Transition> transitions = this->stateList[this->currentState].transitions;

            // Get tape input at TM head and check if it is in state's transition table, reject if not
            char input = this->tape.getAtHead();
            if (transitions.find(input) == transitions.end())
                break;

            // Get transition for the given input and process
            this->currentState = transitions[input].nextState;
            if (transitions[input].goRight)
                this->tape.goRight(transitions[input].write);
            else
                this->tape.goLeft(transitions[input].write);
        }
        // Process result and add to results vector
        if (this->isTransducer)
            addResultTransducer();
        else
            addResult();
    }
    return true;
}

/**
 * Adds result of current test to results vector for recognizer test ("accept" or "reject")
 */
void TuringMachine::addResult()
{
    if (this->isAccepting)
        this->results.push_back("accept");
    else
        this->results.push_back("reject");
}

/**
 * Adds result of current test to results vector for transducer test (contents of tape from head to first blank, moving right)
 */
void TuringMachine::addResultTransducer()
{
    this->results.push_back(tape.transduce());
}

/**
 * Takes in a file line and removes the trailing carriage return (\r) if it exists
 * @param line A reference to a read-in file line
 */
void TuringMachine::cleanLine(string * line)
{
    if (line->back() == '\r')
        line->pop_back();
}

/**
 * Takes in a string and splits it at the specified delimiter, cleaning spaces and parentheses
 * @param line The file line being split
 * @param delim The delimiter that the string is being split along
 */
vector<string> TuringMachine::split(string line, char delim)
{
    vector<string> output;
    string temp;
    for (char c : line) {
        if (c == delim) {
            output.push_back(temp);
            temp.clear();
        } else if (c != ' ' && c != '(' && c != ')') {
            temp += c;
        }
    }
    output.push_back(temp);
    return output;
}

/**
 * Gets vector containing simulation inputs. Indices match those of TuringMachine->results
 * @return TuringMachine->inputs
 */
vector<string> TuringMachine::getInputs()
{
    return this->inputs;
}

/**
 * Gets vector containing simulation results. Indices match those of TuringMachine->inputs
 * @return TuringMachine->results
 */
vector<string> TuringMachine::getResults()
{
    return this->results;
}

/**
 * Gets Turing Machine error stored in TuringMachine->error
 * @return error description as red text
 */
string TuringMachine::getError()
{
    const string COLOR_RED = "\u001B[31m";
    const string COLOR_RESET = "\u001B[0m";
    return COLOR_RED + this->error + COLOR_RESET;
}

/**
 * Runs a Turing Machine simulation given two files containing TM info and inputs, respectively
 */
int main(int argc, char* argv[])
{
    // Validate usage
    if (argc != 3) {
        cout << "\n\tusage: ./tm_simulator <turing machine file> <input file>\n" << endl;
        return 1;
    }
    
    // Load Turing Machine and inputs
    TuringMachine simulator;
    if (!simulator.loadTuringMachine(argv[1])) {
        cout << "\n\tturing_machine.cc::TuringMachine::loadTuringMachine(): " << simulator.getError() << "\n" << endl;
        return 2;
    }
    if (!simulator.loadInputs(argv[2])) {
        cout << "\n\tturing_machine.cc::TuringMachine::loadInputs(): " << simulator.getError() << "\n" << endl;
        return 3;
    }

    // Run Turing Machine
    if (!simulator.run()) {
        cout << "\n\tturing_machine.cc::TuringMachine::run(): " << simulator.getError() << "\n" << endl;
        return 4;
    }

    // Print results
    for (size_t i=0; i < simulator.getResults().size(); i++)
        cout << simulator.getInputs()[i] << " - " << simulator.getResults()[i] << endl;

    return 0;
}
