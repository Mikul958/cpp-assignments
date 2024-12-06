// Copyright 2024 Michael Pikula

#include "turing_machine.h"


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
    // Open tm file
    if (!openFile(filename)) {
        this->error = "tm file \"" + filename + "\" not found";
        return false;
    }

    // Read lines until all comments at top are skipped
    while (getFileLine()) {
        if (this->fileLine.substr(0, 2) != "//")
            break;
    }

    // LINE 0: Check first uncommented line reads "TM"
    if (this->fileLine != "TM") {
        this->error = filename + ", line " + std::to_string(this->lineNumber) +
                    " - unrecognized header \"" + fileLine + "\", expected \"TM\"";
        return false;
    }

    // LINE 1: Add empty states to TM hash map
    getFileLine();
    vector<string> tokens = split(this->fileLine, ',');
    for (string name : tokens) {
        struct State newState = {name};             // Only initialize name for now
        this->stateList.insert({name, newState});   // Add (name, state) pair to hash table
    }

    // LINE 2: Read in input alphabet (sigma)
    getFileLine();
    tokens = split(this->fileLine, ',');
    for (string symbol : tokens)
        this->sigma.push_back(symbol.front());

    // LINE 3: Read in tape alphabet (gamma)
    getFileLine();
    tokens = split(this->fileLine, ',');
    for (string symbol : tokens)
        this->gamma.push_back(symbol.front());

    // LINE 4: Set initial state, ensuring it is already in state list
    getFileLine();
    if (this->stateList.find(this->fileLine) == this->stateList.end()) {
        this->error = filename + ", line " + std::to_string(this->lineNumber) +
                    " - initial state \"" + this->fileLine + "\" not in state list";
        return false;
    }
    this->initialState = this->fileLine;

    // LINE 5: Set blank symbol, ensuring it is already in tape alphabet
    getFileLine();
    if (this->gamma.find(this->fileLine.front()) == string::npos) {
        this->error = filename + ", line " + std::to_string(this->lineNumber) +
                    " - blank symbol \'" + this->fileLine.front() + "\' not in tape alphabet";
        return false;
    }
    this->tape.setBlank(this->fileLine.front());

    // LINE 6: Get list of final states and set all existing states as such, ensuring they are in state list
    getFileLine();
    vector<string> finalStates = split(this->fileLine, ',');
    for (string name : finalStates) {
        if (this->stateList.find(name) == this->stateList.end()) {
            this->error = filename + ", line " + std::to_string(this->lineNumber) +
                         " - final state \"" + name + "\" not in state list";
            return false;
        }
        this->stateList[name].isFinal = true;
    }

    // LINE 7+: Read in deltas and add transitions to states; delta = (currentState, input, nextState, toWrite, direction)
    while (getFileLine()) {
        vector<string> delta = split(this->fileLine, ',');
        if (delta.size() != 5) {
            this->error = filename + ", line " + std::to_string(this->lineNumber) +
                        " - delta is of size " + std::to_string(delta.size()) + ", expected 5";
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

    closeFile();
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
    if (!openFile(filename)) {
        this->error = "input file \"" + filename + "\" not found";
        return false;
    }

    // Read lines until all comments at top are skipped
    while (getFileLine()) {
        if (this->fileLine.substr(0, 2) != "//")
            break;
    }

    // Check first line to ensure file is an input file and check simulation type
    if (this->fileLine == "Recognizer") {
        this->isTransducer = false;
    }
    else if (this->fileLine == "Transducer") {
        this->isTransducer = true;
    }
    else {
        this->error = filename + ", line " + std::to_string(this->lineNumber) +
                      " - unrecognized test type \"" + this->fileLine +
                      "\", expected \"Recognizer\" or \"Transducer\"";
        return false;
    }

    // Load inputs and close file; abort if any input string contains an illegal input
    this->inputs.clear();
    while (getFileLine()) {
        for (char input : this->fileLine) {
            if (this->sigma.find(input) == string::npos) {
                this->error = filename + ", line " + std::to_string(this->lineNumber) +
                            " - found input character \'" + string(1, input) +
                            "\' not in input alphabet specified by Turing Machine";
                return false;
            }
        }
        this->inputs.push_back(this->fileLine);
    }

    closeFile();
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

        // Process tape until TM encounters accepting/final state or dies
        while (true)
        {
            // Test to see if current state is final, halt accept if yes
            if (this->stateList[currentState].isFinal) {
                isAccepting = true;
                break;
            }
            
            // Get current state from hash map using its name, then get its transition table
            unordered_map<char, struct Transition> transitions = this->stateList[currentState].transitions;

            // Get tape input at TM head and check if it is a key in transition table, die if no
            char input = this->tape.getAtHead();
            if (transitions.find(input) == transitions.end())
                break;

            // Otherwise, get transition from table and process
            struct Transition toDo = transitions[input];
            currentState = toDo.nextState;
            if (toDo.goRight)
                this->tape.goRight(toDo.write);
            else
                this->tape.goLeft(toDo.write);
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
 * @param isAccepting whether or not the simulation reached an accepting state
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
 * Opens the file with the given file path and stores in this->currentFile
 * @param filename File path relative to the program directory
 * @return true if file was successfully opened
 */
bool TuringMachine::openFile(string filename)
{
    closeFile();
    currentFile.open(filename.c_str());
    if (!currentFile.is_open())
        return false;
    return true;
}

/**
 * Closes the file open at this->currentFile, if it exists
 */
void TuringMachine::closeFile()
{
    if (currentFile.is_open())
        currentFile.close();
    lineNumber = -1;
}

/**
 * Wrapper for std::getline(this->currentFile, &this->fileLine); tracks line number and cleans trailing '\r' from line
 * @return true if there was a file line available to read
 */
bool TuringMachine::getFileLine()
{
    // Check file is open and has another line to read
    if (!currentFile.is_open() || !std::getline(this->currentFile, this->fileLine))
        return false;
    
    // Clean trailing '\r' if it exists
    lineNumber++;
    if (fileLine.back() == '\r')
        fileLine.pop_back();
    return true;
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
 * Gets error thrown by Turing Machine, if it exists
 * @return error message in format (header + ": " + error)
 */
string TuringMachine::getError()
{
    return this->error;
}