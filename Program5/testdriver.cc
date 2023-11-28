// Copyright 2023 mpikula
// Code used to test each written class.

#include "question.h"
#include "truefalsequestion.h"
#include "truefalsequestion.cc"

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

    TrueFalseQuestion tf;
    tf.Print(false);
    cout << endl;
    tf.Print(true);

    cout << "-----------------------------------" << endl;

    TrueFalseQuestion tf_param("Am I hot sexy?!?!!", false);
    tf_param.Print(true);

    cout << endl;
    
    return 0;
}