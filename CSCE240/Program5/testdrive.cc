// Copyright 2023 mpikula
// Code used to test each written class.

#include "question.h"
#include "truefalsequestion.h"
#include "truefalsequestion.cc"         // For VS Code testing
#include "shortanswerquestion.h"
#include "shortanswerquestion.cc"       // For VS Code testing
#include "multiplechoicequestion.h"
#include "multiplechoicequestion.cc"    // For VS Code testing

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;

using namespace csce240_program5;

int main()
{
    cout << "START" << endl;
    
    // Testing base Question class
    cout << "----------------------------------- TEST QUESTION" << endl;
    Question q;
    q.Print();

    // Testing TrueFalseQuestion
    cout << "----------------------------------- TEST TF" << endl;
    TrueFalseQuestion tf("Am I hot sexy?!?!!", false);
    tf.Print(false);
    cout << endl;
    tf.Print(true);

    // Testing ShortAnswerQuestion
    cout << "----------------------------------- TEST SA" << endl;
    ShortAnswerQuestion sa("On a scale of 1-10, how cool am I?", "9");
    sa.Print(false);
    cout << endl;
    sa.Print(true);

    // Testing MultipleChoiceQuestion
    cout << "----------------------------------- TEST MC" << endl;
    string choices[] = {"Balloon", "Ball", "Dapper", "Bed"};
    bool answers[] = {true, true, false, true};
    MultipleChoiceQuestion mc("Which words start with B?", 4, choices, answers);

    mc.Print(false);
    cout << endl;
    mc.Print(true);

    // Testing MC copy constructor.
    cout << "----------------------------------- TEST MC COPY" << endl;
    MultipleChoiceQuestion mc_copy = mc;

    string new_choices[] = {"Air", "Eat", "Barge"};
    bool new_answers[] = {false, false, true,};
    mc.SetAnswerChoices(3, new_choices, new_answers);

    mc.Print(true);         // Should print new answers.
    cout << endl;
    mc_copy.Print(true);    // Should print old answers.

    // Testing MC assignment operator
    cout << "----------------------------------- TEST MC ASSIGNMENT" << endl;
    mc_copy = mc;

    mc.Print(true);
    cout << endl;
    mc_copy.Print(true);    // Should both print new answers.

    string newer_choices[] = {"Air", "Dog", "Cat", "Ostrich", "Sailboat"};
    bool newer_answers[] = {false, false, false, false, false};

    cout << "\n--- SET MC_COPY ---\n" << endl;

    mc_copy.SetAnswerChoices(5, newer_choices, newer_answers);

    mc.Print(true);         // Should print new answers.
    cout << endl;
    mc_copy.Print(true);    // Should print newer answers.

    cout << endl;
    
    return 0;
}