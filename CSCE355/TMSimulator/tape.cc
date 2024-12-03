// Copyright 2024 Michael Pikula

#include "tape.h"

// Add initial element to tape vector and set head and tapeSize
Tape::Tape()
{
    head = 0;
    tapeSize = 1;
    tape.push_back(kBlank);
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

// FOR TESTING ONLY; prints all contents of tape
void Tape::printTape()
{
    std::string output = "";
    for (int i=0; i < tapeSize; i++) {
        if (tape[i] == kBlank)
            output += "_";
        else
            output += tape[i];
    }

    std::cout << output << std::endl;
}

// FOR TESTING ONLY;
int main(int argc, char* argv[])
{
    Tape t;

    for (int i=1; i < argc; i++) {
        if (argv[i][2] == 'L')
            t.goLeft(argv[i][0]);
        else
            t.goRight(argv[i][0]);
    }

    t.printTape();
}