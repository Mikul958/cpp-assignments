// Copyright 2024 Michael Pikula

#include "tape.h"

/**
 * Sets the symbol the tape recognizes as blank to the specified character
 * @param newBlank Character to use as blank symbol
 */
void Tape::setBlank(char newBlank)
{
    blank = newBlank;
}

// Resets tape, and, optionally loads an input string onto it
/**
 * Clears the tape and resets tracking information, then loads the provided string onto the tape
 * @param toLoad Input string to load onto the tape; empty by default
 */
void Tape::resetTape(string toLoad)
{
    head = 0;
    tapeSize = 1;
    tape.clear();
    tape.push_back(blank);

    // Write new input string to tape and reset head back to start
    for (char input : toLoad)
        goRight(input);
    head = 0;
}

/**
 * Gets the character on the tape at the Turing Machine head
 * @return Character at head's location
 */
char Tape::getAtHead()
{
    return tape[head];
}

/**
 * Marks the current tape cell and moves the head to the left, adding more space if needed
 * @param newSymbol Character to mark the current cell with
 */
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

/**
 * Marks the current tape cell and moves the head to the right, adding more space if needed
 * @param newSymbol Character to mark the current cell with
 */
void Tape::goRight(char newSymbol)
{
    tape[head++] = newSymbol;

    // Add blank symbol at new head location if right boundary is crossed
    if (head >= tapeSize) {
        tape.push_back(blank);
        tapeSize++;
    }
}

/**
 * Returns the output of a transducer test as a string
 * @return A string containing all contents of the tape from the head to the first blank, moving right
 */
string Tape::transduce()
{
    string output = "";
    while (head < tapeSize && tape[head] != blank) {
        output += tape[head];
        head++;
    }
    return output;
}