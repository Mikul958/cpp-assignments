// Copyright 2024 Michael Pikula

#ifndef _TAPE_H_
#define _TAPE_H_

#include <vector>

// TODO remove later
#include <iostream>
#include <string>

const char kBlank = '\037';

// Class representing the tape of a turing machine
class Tape
{
    public:
        explicit Tape();

        void goLeft(char);
        void goRight(char);

        void printTape();

    private:
        int head;                 // Index of head
        int tapeSize;             // Running size of tape
        std::vector<char> tape;   // Tape with initial length
};

#endif  // _TAPE_H_