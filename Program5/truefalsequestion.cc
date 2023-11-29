// Copyright 2023 mpikula
// Implementation of the TrueFalseQuestion child class.

#include "truefalsequestion.h"
#include <iostream>
using std::cout;
using std::endl;

namespace csce240_program5
{

// Member initializer list calls parent class constructor, analagous to Java super().
TrueFalseQuestion::TrueFalseQuestion(string question, bool answer) : Question(question)
{
    answer_ = answer;
}

bool TrueFalseQuestion::GetAnswer() const
{
    return answer_;
}
void TrueFalseQuestion::SetAnswer(bool answer)
{
    answer_ = answer;
}

void TrueFalseQuestion::Print(bool show_answer) const
{
    cout << "Question: " << GetQuestion() << endl;
    if (show_answer)
        cout << "Correct Answer: " << (answer_ ? "true" : "false") << endl;
}

}  // namespace csce240_program5
