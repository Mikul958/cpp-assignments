// Copyright 2024 Michael Pikula

#include "turing_machine.h"

#include <iostream>

// Initialize all states of Turing machine using given TM text file, returns true if successful
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
        bool headRight = (delta[4] == "R" ? true : false);

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

// Load inputs using given input text file, returns true if successful
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

// Runs the Turing machine simulation for each input string and stores the results
bool TuringMachine::run()
{
    for (string inputString : inputs)
    {
        // Reset all ephemeral properties
        currentState = initialState;
        tape.resetTape();
        isAccepting = false;

        std::cout << "Processing input string: " << inputString << std::endl;

        // Process each input in input string
        for (char input : inputString)
        {
            // Get current state from list using its name and halt if it is final
            if (stateList[currentState].isFinal) {
                isAccepting = true;
                break;
            }
            
            // Get current state from hash map based on its name, then get its transition table
            unordered_map<char, struct Transition> transitions = stateList[currentState].transitions;

            // Check if given input is in state's transition table, reject if not
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

        std::cout << "Resulting Tape: " << tape.getWholeTape() << std::endl;
    }

    return true;
}

// Adds result to results vector for recognizer tests
void TuringMachine::addResult()
{
    if (isAccepting)
        results.push_back("accept");
    else
        results.push_back("reject");
}

// Adds result to results vector for transducer tests
void TuringMachine::addResultTransducer()
{
    results.push_back(tape.transduce());
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

vector<string> TuringMachine::getInputs() {
    return inputs;
}

vector<string> TuringMachine::getResults() {
    return results;
}

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

