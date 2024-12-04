// Copyright 2024 Michael Pikula

#include "turing_machine.h"

#include <iostream>

/**
 * Initializes states, alphabet, and blank symbol of Turing Machine using given TM text file
 * @param filename Path of a text file containing Turing Machine info (Ex. data/wwr_tm.txt)
 * @return true if load successful, false otherwise
 */
bool TuringMachine::loadTM(string filename)
{
    // Open file using ifstream
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        return false;

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
        stateList.insert({name, newState});
    }

    // LINE 2: Read in input alphabet (sigma)
    std::getline(file, currentLine);
    cleanLine(&currentLine);

    tokens = split(currentLine, ',');
    for (string symbol : tokens)
        sigma.push_back(symbol.front());

    // LINE 3: Read in tape alphabet (gamma)
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    
    tokens = split(currentLine, ',');
    for (string symbol : tokens)
        gamma.push_back(symbol.front());

    // LINE 4: Set initial state
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    initialState = currentLine;

    // LINE 5: Set blank symbol
    std::getline(file, currentLine);
    cleanLine(&currentLine);
    tape.setBlank(currentLine.front());

    // LINE 6: Get list of final states and set all existing states as such
    std::getline(file, currentLine);
    cleanLine(&currentLine);

    vector<string> finalStates = split(currentLine, ',');
    for (string name : finalStates)
        stateList[name].isFinal = true;

    // LINE 7+: Read deltas and set up transitions (current state name, input, next state name, char to write, head direction)
    while (std::getline(file, currentLine)) {
        cleanLine(&currentLine);
        vector<string> delta = split(currentLine, ',');

        // These are necessary for readability, it sucks otherwise
        string currentName = delta[0];
        char input = delta[1].front();
        string nextName = delta[2];
        char toWrite = delta[3].front();
        bool headRight = (delta[4] == "R");

        // Create new transition and add at specified state + input
        struct Transition newTransition = {nextName, toWrite, headRight};
        stateList[currentName].transitions.insert({input, newTransition});

        // TODO return false if input is not part of tape alphabet
    }



    // TEST EVERYTHING
    std::cout << "LOADED TURING MACHINE:\n-----------------------" << std::endl;
    std::cout << "INPUT ALPHABET: ";
    for (char c : sigma)
        std::cout << c << ", ";
    std::cout << "\nTAPE ALPHABET: ";
    for (char c : gamma)
        std::cout << c << ", ";
    std::cout << std::endl;

    for (auto statePair : stateList) {
        std::cout << "STATE: " << statePair.first << std::endl;
        std::cout << "\tName: " << statePair.second.name << std::endl;
        std::cout << "\tisFinal: " << (statePair.second.isFinal ? "Yes" : "No") << std::endl;
        std::cout << "\tTRANSITIONS:" << std::endl;
        for (auto transitionPair : statePair.second.transitions) {
            std::cout << "\t\tON INPUT: " << transitionPair.first << std::endl;
            std::cout << "\t\t\tNext: " << transitionPair.second.nextState << std::endl;
            std::cout << "\t\t\tWrite: " << transitionPair.second.write << std::endl;
            std::cout << "\t\t\tDirection: " << (transitionPair.second.goRight ? "Right" : "Left") << std::endl;
        }
    }
    std::cout << "INITIAL STATE: " << initialState << "\n----------------------------------------------------" << std::endl;
    


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
    if (!file.is_open())
        return false;

    // Get and clean first line of file
    string currentLine;
    std::getline(file, currentLine);
    cleanLine(&currentLine);

    // Check first line to ensure file is an input file and check simulation type
    if (currentLine == "Recognizer")
        isTransducer = false;
    else if (currentLine == "Transducer")
        isTransducer = true;
    else
        return false;

    // Load inputs and close file
    while (std::getline(file, currentLine)) {
        cleanLine(&currentLine);
        inputs.push_back(currentLine);

        // TODO return false if string contains char not in input alphabet
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
    for (string inputString : inputs)
    {
        // Reset all ephemeral properties
        currentState = initialState;
        tape.resetTape(inputString);  // Reset tape and load current input string onto it
        isAccepting = false;

        std::cout << "Processing input string: " << inputString << std::endl;

        // Process each input in input string
        while (true)
        {
            // Get current state from list using its name and accept if it is final
            if (stateList[currentState].isFinal) {
                isAccepting = true;
                break;
            }
            
            // Get current state from hash map based on its name, then get its transition table
            unordered_map<char, struct Transition> transitions = stateList[currentState].transitions;

            // Get tape input at TM head and check if it is in state's transition table, reject if not
            char input = tape.getAtHead();
            if (transitions.find(input) == transitions.end())
                break;



            // TESTING PRINT
            std::cout << "\tINPUT:" << input << std::endl;
            std::cout << "\t\tCurrent State: " << currentState << std::endl;
            std::cout << "\t\tCurrent Input:" << input << std::endl;
            std::cout << "\t\t\tNext State:" << transitions[input].nextState << std::endl;
            std::cout << "\t\t\tWriting:" << transitions[input].write << std::endl;
            std::cout << "\t\t\tHead Going: " << (transitions[input].goRight ? "right" : "left") << std::endl;



            // Get transition for the given input and process
            currentState = transitions[input].nextState;
            if (transitions[input].goRight)
                tape.goRight(transitions[input].write);
            else
                tape.goLeft(transitions[input].write);
        }
        // Process result and add to results vector
        if (isTransducer)
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
    if (isAccepting)
        results.push_back("accept");
    else
        results.push_back("reject");
}

/**
 * Adds result of current test to results vector for transducer test (contents of tape from head to first blank, moving right)
 */
void TuringMachine::addResultTransducer()
{
    results.push_back(tape.transduce());
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

// TODO do we need?
vector<string> TuringMachine::getInputs()
{
    return inputs;
}

// TODO do we need?
vector<string> TuringMachine::getResults()
{
    return results;
}

/**
 * Runs a Turing Machine simulation given two files containing TM info and inputs, respectively
 */
int main()
{
    // Initialize simulator and load files
    TuringMachine simulator;
    simulator.loadTM("data/wwr_tm.txt");
    simulator.loadInputs("data/wwr_input.txt");

    // Run
    simulator.run();

    // Print results
    for (size_t i=0; i < simulator.getResults().size(); i++)
    {
        std::cout << simulator.getInputs()[i] << " - " << simulator.getResults()[i] << std::endl;
    }
}

