// Copyright 2024 Michael Pikula

#include "tape.h"

// Add initial element to tape vector and set head and tapeSize
Tape::Tape()
{
    blank = '_';  // Default blank symbol, changed later during TM initialization
    resetTape();
}

// Sets the tape's blank symbol to the specified character
void Tape::setBlank(char newBlank)
{
    blank = newBlank;
}

// Resets tape, and, optionally loads an input string onto it
void Tape::resetTape(string toLoad)
{
    head = 0;
    tapeSize = 1;
    tape.clear();
    tape.push_back(blank);

    for (char input : toLoad)
        goRight(input);
    head = 0;
}

char Tape::getAtHead()
{
    return tape[head];
}

// Mark current cell with new symbol and move left
void Tape::goLeft(char newSymbol)
{   
    tape[head--] = newSymbol;

    // Add blank symbol at new head location if left boundary is crossed
    if (head < 0) {
        tape.insert(tape.begin(), blank);
        head = 0;
        tapeSize++;
    }
}

// Mark current cell with new symbol and move right
void Tape::goRight(char newSymbol)
{
    tape[head++] = newSymbol;

    // Add blank symbol at new head location if right boundary is crossed
    if (head >= tapeSize) {
        tape.push_back(blank);
        tapeSize++;
    }
}

// Returns contents to right of current head until first blank on tape
string Tape::transduce()
{
    string output = "";
    while (head < tapeSize && tape[head] != blank) {
        output += tape[head];
        head++;
    }
    return output;
}

string Tape::getWholeTape()
{
    string output;
    for (char c : tape)
        output += c;
    return output;
}