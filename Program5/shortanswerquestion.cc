// Copyright 2023 mpikula
// Implementation of the ShortAnswerQuestion child class.

#include "shortanswerquestion.h"
#include <iostream>
using std::cout;
using std::endl;

namespace csce240_program5
{

// Member initializer list calls parent class constructor.
ShortAnswerQuestion::ShortAnswerQuestion(string question, string answer) : Question(question)
{
    answer_ = answer;
}

string ShortAnswerQuestion::GetAnswer() const
{
    return answer_;
}
void ShortAnswerQuestion::SetAnswer(string answer)
{
    answer_ = answer;
}

void ShortAnswerQuestion::Print(bool show_answer) const
{
    cout << "Question: " << GetQuestion() << endl;
    if (show_answer)
        cout << "Correct Answer: " << answer_ << endl;
}

}  // namespace csce240_program5