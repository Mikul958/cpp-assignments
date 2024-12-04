// Copyright 2024 Michael Pikula

#include "turing_machine.h"

#include <iostream>

// Initialize all states of Turing machine using given TM text file
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
    currentState = currentLine;

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
        bool headRight = (delta[4] == "R" ? true : false);

        // Create new transition and add at specified state + input
        struct Transition newTransition = {nextName, toWrite, headRight};
        stateList[currentName].transitions.insert({input, newTransition});
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
            std::cout << "\t\t\tNext: " << transitionPair.second.next << std::endl;
            std::cout << "\t\t\tWrite: " << transitionPair.second.write << std::endl;
            std::cout << "\t\t\tDirection: " << (transitionPair.second.goRight ? "Right" : "Left") << std::endl;
        }
    }
    std::cout << "INITIAL STATE: " << currentState << "\n----------------------------------------------------" << std::endl;
    


    return true;
}

// Simulate Turing machine using given input text file
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
    }
    file.close();



    // TEST EVERYTHING
    std::cout << "LOADED INPUTS:" << std::endl;
    for (string s : inputs)
        std::cout << " - " << s << std::endl;



    return true;
}

bool TuringMachine::run()
{
    // TODO
    return true;
}

// Takes in a file line and cleans the carriage return (\r) if it exists
void TuringMachine::cleanLine(string * line)
{
    if (line->back() == '\r')
        line->pop_back();
}

// Takes in a file line and splits at the specified delimiter, ignoring whitespace and parentheses
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

int main() {
    TuringMachine m;
    m.loadTM("data/wwr_tm.txt");
    m.loadInputs("data/wwr_input.txt");
}

