// Copyright 2024 Michael Pikula

#include "tape.h"

// Add initial element to tape vector and set head and tapeSize
Tape::Tape()
{
    resetTape();
    blank = '_';  // Default blank symbol, changed later during TM initialization
}

// Sets the tape's blank symbol to the specified character
void Tape::setBlank(char newBlank)
{
    blank = newBlank;
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

// Resets (or initializes) tape
void Tape::resetTape() {
    tape.clear();
    head = 0;
    tapeSize = 1;
    tape.push_back(blank);
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