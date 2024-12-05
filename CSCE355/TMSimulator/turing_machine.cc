// Copyright 2024 Michael Pikula

#include "turing_machine.h"

#include <iostream>
using std::cout;
using std::endl;


/**
 * Initializes TuringMachine using LoadTuringMachine() and loadInputs()
 * @param tmFilename File path of the file containing turing machine information
 * @param inputFilename File path of the file containing test type and input strings
 */
TuringMachine::TuringMachine(string tmFilename, string inputFilename)
{
    if (!loadTuringMachine(tmFilename))
        return;
    if (!loadInputs(inputFilename))
        return;
    this->isOK = true;
}

/**
 * Initializes states, alphabet, and blank symbol of Turing Machine using given TM text file.
 * @param filename Path of a text file containing Turing Machine info (Ex. data/wwr_tm.txt)
 * @return true if load successful, false otherwise
 */
bool TuringMachine::loadTuringMachine(string filename)
{
    // Open file using ifstream
    int lineNumber = -1;
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        this->error = "file \"" + filename + "\' not found";
        return false;
    }

    // Read lines until all comments are skipped
    string currentLine;
    while (std::getline(file, currentLine)) {
        lineNumber++;
        if (currentLine.substr(0, 2) != "//")
            break;
    }

    // LINE 0: Check first uncommented line reads "TM"
    cleanLine(&currentLine);
    if (currentLine != "TM") {
        this->error = filename + ", line " + std::to_string(lineNumber) +
                    ": unrecognized header \"" + currentLine + "\", expected \"TM\"";
        return false;
    }

    // LINE 1: Add empty states to TM hash map
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    lineNumber++;

    vector<string> tokens = split(currentLine, ',');
    for (string name : tokens) {
        struct State newState = {name};     // Create new State with name of current string
        this->stateList.insert({name, newState});
    }

    // LINE 2: Read in input alphabet (sigma)
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    lineNumber++;

    tokens = split(currentLine, ',');
    for (string symbol : tokens)
        this->sigma.push_back(symbol.front());

    // LINE 3: Read in tape alphabet (gamma)
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    lineNumber++;
    
    tokens = split(currentLine, ',');
    for (string symbol : tokens)
        this->gamma.push_back(symbol.front());

    // LINE 4: Set initial state, ensuring it is already in state list
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    lineNumber++;

    if (this->stateList.find(currentLine) == this->stateList.end()) {
        this->error = filename + ", line " + std::to_string(lineNumber) +
                    ": initial state \"" + currentLine + "\" not in state list";
        return false;
    }
    this->initialState = currentLine;

    // LINE 5: Set blank symbol, ensuring it is already in tape alphabet
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    lineNumber++;

    if (this->gamma.find(currentLine.front()) == string::npos) {
        this->error = filename + ", line " + std::to_string(lineNumber) +
                    ": blank symbol \'" + currentLine.front() + "\' not in tape alphabet";
        return false;
    }
    this->tape.setBlank(currentLine.front());

    // LINE 6: Get list of final states and set all existing states as such, ensuring they are in state list
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    lineNumber++;

    vector<string> finalStates = split(currentLine, ',');
    for (string name : finalStates) {
        if (this->stateList.find(name) == this->stateList.end()) {
            this-> error = filename + ", line " + std::to_string(lineNumber) +
                         ": final state \"" + name + "\" not in state list";
            return false;
        }
        this->stateList[name].isFinal = true;
    }

    // LINE 7+: Read in deltas and add transitions to states; delta = (currentState, input, nextState, toWrite, direction)
    while (std::getline(file, currentLine)) {
        cleanLine(&currentLine);
        lineNumber++;
        vector<string> delta = split(currentLine, ',');
        if (delta.size() != 5) {
            this->error = filename + ", line " + std::to_string(lineNumber) +
                        ": delta is of size " + std::to_string(delta.size()) + ", expected 5";
            return false;
        }

        // Retrieve information as correct data types from split delta (too lazy to validate these)
        string currentName = delta[0];
        char input = delta[1].front();
        string nextName = delta[2];
        char toWrite = delta[3].front();
        bool headRight = (delta[4] == "R");

        // Create new transition and add at specified state + input
        struct Transition newTransition = {nextName, toWrite, headRight};
        this->stateList[currentName].transitions.insert({input, newTransition});
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
    int lineNumber = -1;
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        this->error = "file \"" + filename + "\' not found";
        return false;
    }

    // Read lines until all comments are skipped
    string currentLine;
    while (std::getline(file, currentLine)) {
        lineNumber++;
        if (currentLine.substr(0, 2) != "//")
            break;
    }

    // Check first line to ensure file is an input file and check simulation type
    cleanLine(&currentLine);
    if (currentLine == "Recognizer") {
        this->isTransducer = false;
    }
    else if (currentLine == "Transducer") {
        this->isTransducer = true;
    }
    else {
        this->error = filename + ", line " + std::to_string(lineNumber) +
                      ": unrecognized test type \"" + currentLine +
                      "\", expected \"Recognizer\" or \"Transducer\"";
        return false;
    }

    // Load inputs and close file
    this->inputs.clear();
    while (std::getline(file, currentLine)) {
        lineNumber++;
        cleanLine(&currentLine);
        this->inputs.push_back(currentLine);

        // Abort if input string contains a character not in input alphabet
        for (char input : currentLine) {
            if (sigma.find(input) == string::npos) {
                this->error = filename + ", line " + std::to_string(lineNumber) +
                            ": found input character \'" + string(1, input) +
                            "\' not in input alphabet specified by Turing Machine";
                return false;
            }
        }
    }
    file.close();
    return true;
}


/**
 * Simulates a Turing Machine test for all input strings.
 * @return true if Turing Machine was successfully initialized, false otherwise
 */
bool TuringMachine::run()
{
    // Abort if turing machine was not initialized
    if (!this->isOK)
        return false;
    
    // Declare variables to track state and status of current run, run tests for all strings
    string currentState;
    bool isAccepting;
    for (string inputString : this->inputs)
    {
        // Initialize values for the current run
        currentState = this->initialState;
        isAccepting = false;
        this->tape.resetTape(inputString);    // Reset TM tape and load new input string onto it

        // Process tape inputs until halt accept or reject
        while (true)
        {
            // Test to see if current state is final, halt accept if yes
            if (this->stateList[currentState].isFinal) {
                isAccepting = true;
                break;
            }
            
            // Get current state from hash map using its name, then get its transition table
            unordered_map<char, struct Transition> transitions = this->stateList[currentState].transitions;

            // Get tape input at TM head and check if it is in state's transition table, reject if not
            char input = this->tape.getAtHead();
            if (transitions.find(input) == transitions.end())
                break;

            // Get transition for the given input and process
            currentState = transitions[input].nextState;
            if (transitions[input].goRight)
                this->tape.goRight(transitions[input].write);
            else
                this->tape.goLeft(transitions[input].write);
        }
        // Process result and add to results vector
        if (this->isTransducer)
            addResultTransducer();
        else
            addResultRecognizer(isAccepting);
    }
    return true;
}

/**
 * Adds result of current test to results vector for recognizer test ("accept" or "reject")
 */
void TuringMachine::addResultRecognizer(bool isAccepting)
{
    if (isAccepting)
        this->results.push_back("accept");
    else
        this->results.push_back("reject");
}

/**
 * Adds result of current test to results vector for transducer test (contents of tape from head to first blank, moving right)
 */
void TuringMachine::addResultTransducer()
{
    this->results.push_back(this->tape.transduce());
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
 * @return A copy of TuringMachine->inputs
 */
vector<string> TuringMachine::getInputs()
{
    return this->inputs;
}

/**
 * Gets vector containing simulation results. Indices match those of TuringMachine->inputs
 * @return A copy of TuringMachine->results
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
    
    // Initialize TuringMachine instance using tm and input files and attempt to run
    TuringMachine simulator(argv[1], argv[2]);
    if (!simulator.run()) {
        cout << "\n\tFAILED TO INITIALIZE TURING MACHINE:\n\t" << simulator.getError() << "\n" << endl;
        return 3;
    }

    // Print results
    for (size_t i=0; i < simulator.getResults().size(); i++)
        cout << simulator.getInputs()[i] << " - " << simulator.getResults()[i] << endl;

    return 0;
}
