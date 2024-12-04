// Copyright 2024 Michael Pikula

#ifndef _TAPE_H_
#define _TAPE_H_

#include <vector>
#include <string>

using std::vector;
using std::string;

// Class representing the tape of a turing machine
class Tape
{
    public:
        explicit Tape();
        void setBlank(char);

        void goLeft(char);
        void goRight(char);

        void resetTape();
        string getContents();

    private:
        int head;                 // Index of head
        int tapeSize;             // Running size of tape
        char blank;               // Blank character specified by TM file
        vector<char> tape;        // Contents of tape
};

#endif  // _TAPE_H_