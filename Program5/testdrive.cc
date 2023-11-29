// Copyright 2023 mpikula
// Code used to test each written class.

#include "question.h"
#include "truefalsequestion.h"
#include "truefalsequestion.cc"
#include "shortanswerquestion.h"
#include "shortanswerquestion.cc"

#include <iostream>
using std::cout;
using std::endl;

using namespace csce240_program5;

int main()
{
    cout << endl;
    
    Question q;
    q.Print();

    cout << "-----------------------------------" << endl;

    TrueFalseQuestion tf("Am I hot sexy?!?!!", false);
    tf.Print(false);
    cout << endl;
    tf.Print(true);

    cout << "-----------------------------------" << endl;

    ShortAnswerQuestion sa("On a scale of 1-10, how cool am I?", "9");
    sa.Print(false);
    cout << endl;
    sa.Print(true);

    cout << endl;
    
    return 0;
}