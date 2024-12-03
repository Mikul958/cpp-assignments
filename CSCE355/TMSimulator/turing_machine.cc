// Copyright 2024 Michael Pikula

#include "turing_machine.h"

#include <iostream>

// Initialize all states of Turing machine using given TM text file
bool TuringMachine::LoadTM(string filename)
{
    // Open file using ifstream
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        return false;
    
    // Check first line to ensure file is a TM file
    string currentLine;
    std::getline(file, currentLine);
    if (currentLine != "TM")
        return false;
    
    // TODO figure out the rest


    return true;
}

// Simulate Turing machine using given input text file
bool TuringMachine::LoadInputs(string filename)
{
    // Open file using ifstream
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        return false;

    // Get and clean first line of file
    string currentLine;
    std::getline(file, currentLine);
    if (currentLine.back() == '\r')
        currentLine.pop_back();

    // Check first line to ensure file is an input file and check simulation type
    if (currentLine == "Recognizer")
        isTransducer = false;
    else if (currentLine == "Transducer")
        isTransducer = true;
    else
        return false;

    // Load inputs and close file
    while (std::getline(file, currentLine)) {
        if (currentLine.back() == '\r')
            currentLine.pop_back();
        inputs.push_back(currentLine);
    }
    file.close();
    return true;
}

bool TuringMachine::Run()
{
    // TODO
    return true;
}

int main() {
    TuringMachine m;
    m.LoadInputs("data/wwr_input.txt");
}

