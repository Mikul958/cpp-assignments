// Copyright 2024 Michael Pikula

#include "tape.h"

// Add initial element to tape vector and set head and tapeSize
Tape::Tape()
{
    resetTape();
}

// Mark current cell with new symbol and move left
void Tape::goLeft(char newSymbol)
{   
    tape[head--] = newSymbol;

    // Add blank symbol to left of tape if left boundary is crossed
    if (head < 0) {
        tape.insert(tape.begin(), kBlank);
        head = 0;
        tapeSize++;
    }
}

// Mark current cell with new symbol and move right
void Tape::goRight(char newSymbol)
{
    tape[head++] = newSymbol;

    // Add blank symbol to right of tape if right boundary is crossed
    if (head >= tapeSize) {
        tape.push_back(kBlank);
        tapeSize++;
    }
}

// Resets (or initializes) tape
void Tape::resetTape() {
    tape.clear();
    head = 0;
    tapeSize = 1;
    tape.push_back(kBlank);
}

// Returns contents right of current head until first blank on tape
string Tape::getContents()
{
    string output = "";
    while (head < tapeSize && tape[head] != kBlank) {
        output += tape[head];
        head++;
    }
    return output;
}