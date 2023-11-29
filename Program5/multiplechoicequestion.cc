// Copyright 2023 mpikula
// Implementation of the MultipleChoiceQuestion child class.

#include "multiplechoicequestion.h"
#include <iostream>
using std::cout;
using std::endl;

namespace csce240_program5
{

MultipleChoiceQuestion::MultipleChoiceQuestion(string q, int num, string * c, bool * ans) : Question(q)
{
    choices_ = new string[num];
    answers_ = new bool[num];
    SetAnswerChoices(num, c, ans);
}

MultipleChoiceQuestion::MultipleChoiceQuestion(const MultipleChoiceQuestion& toCopy) : Question(toCopy.GetQuestion())
{
    num_choices_ = toCopy.num_choices_;
    choices_ = new string[num_choices_];
    answers_ = new bool[num_choices_];

    for (int i=0; i<num_choices_; i++)
    {
        choices_[i] = toCopy.choices_[i];
        answers_[i] = toCopy.answers_[i];
    }
}
MultipleChoiceQuestion& MultipleChoiceQuestion::operator = (const MultipleChoiceQuestion& right)
{
    SetQuestion(right.GetQuestion());
    num_choices_ = right.num_choices_;
    delete [] choices_;
    delete [] answers_;
    choices_ = new string[num_choices_];
    answers_ = new bool[num_choices_];
    for (int i=0; i<num_choices_; i++)
    {
        choices_[i] = right.choices_[i];
        answers_[i] = right.answers_[i];
    }

    return *this;
}
MultipleChoiceQuestion::~MultipleChoiceQuestion()
{
    delete [] choices_;
    delete [] answers_;
}

int MultipleChoiceQuestion::GetNumChoices() const
{
    return num_choices_;
}
void MultipleChoiceQuestion::SetAnswerChoices(int num, string * c, bool * ans)
{
    num_choices_ = num;

    if (c == nullptr)
    {
        choices_ = new string[num];
        for (int i=0; i<num; i++)
            choices_[i] = "?";
    }
    else
    {
        choices_ = c;
    }

    if (ans == nullptr)
    {
        answers_ = new bool[num];
        for (int i=0; i<num; i++)
            answers_[i] = true;
    }
    else
    {
        answers_ = ans;
    }
}

void MultipleChoiceQuestion::Print(bool show_answers) const
{
    cout << "Question: " << GetQuestion() << endl;
    cout << "Answer Choices:" << endl;
    for (int i=0; i<num_choices_; i++)
    {
        cout << choices_[i];
        if (show_answers)
            cout << (answers_[i] ? " - correct" : " - incorrect");
        cout << endl;
    }
}

}  // namespace csce240_program5