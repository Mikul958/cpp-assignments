// Copyright 2024 Michael Pikula

#ifndef _TAPE_H_
#define _TAPE_H_

#include <vector>
#include <string>

using std::vector;
using std::string;


/**
 * Class representing the tape and head of a Turing Machine
 * @author Michael Pikula
 */
class Tape
{
    public:
        void setBlank(char);

        void resetTape(string = "");
        char getAtHead();

        void goLeft(char);
        void goRight(char);

        string transduce();

    private:        
        int head;                 // Index of head
        int tapeSize;             // Running size of tape
        char blank;               // Blank character specified by definition file
        string tape;              // Contents of tape
};

#endif  // _TAPE_H_