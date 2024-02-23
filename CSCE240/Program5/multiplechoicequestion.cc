// Copyright 2023 mpikula
// Implementation of the MultipleChoiceQuestion child class.

#include "multiplechoicequestion.h"
#include <iostream>
using std::cout;
using std::endl;

namespace csce240_program5 {

MultipleChoiceQuestion::MultipleChoiceQuestion(string q, int num, string * c,
                                               bool * ans) : Question(q) {
    num_choices_ = 0;
    choices_ = new string[1];
    answers_ = new bool[1];
    choices_[0] = "";
    answers_[0] = true;
    SetAnswerChoices(num, c, ans);
}

MultipleChoiceQuestion::MultipleChoiceQuestion
                        (const MultipleChoiceQuestion& toCopy)
                        : Question(toCopy.GetQuestion()) {
    num_choices_ = toCopy.num_choices_;
    choices_ = new string[num_choices_];
    answers_ = new bool[num_choices_];

    for (int i=0; i < num_choices_; i++) {
        choices_[i] = toCopy.choices_[i];
        answers_[i] = toCopy.answers_[i];
    }
}
MultipleChoiceQuestion& MultipleChoiceQuestion::operator =
                        (const MultipleChoiceQuestion& right) {
    SetQuestion(right.GetQuestion());
    num_choices_ = right.num_choices_;
    delete [] choices_;
    delete [] answers_;
    choices_ = new string[num_choices_];
    answers_ = new bool[num_choices_];
    for (int i=0; i < num_choices_; i++) {
        choices_[i] = right.choices_[i];
        answers_[i] = right.answers_[i];
    }

    return *this;
}
MultipleChoiceQuestion::~MultipleChoiceQuestion() {
    delete [] choices_;
    delete [] answers_;
}

int MultipleChoiceQuestion::GetNumChoices() const {
    return num_choices_;
}
void MultipleChoiceQuestion::SetAnswerChoices(int num, string * c, bool * ans) {
    if (num < 1)
        return;
    num_choices_ = num;
    delete [] choices_;
    delete [] answers_;
    choices_ = new string[num_choices_];
    answers_ = new bool[num_choices_];

    for (int i=0; i < num_choices_; i++) {
        // If answer choices parameter is a null pointer, default choices to ""
        if (c == nullptr)
            choices_[i] = "";
        else
            choices_[i] = c[i];

        // If answers parameter is a null pointer, default all answers to true.
        if (ans == nullptr)
            answers_[i] = true;
        else
            answers_[i] = ans[i];
    }
}

void MultipleChoiceQuestion::Print(bool show_answers) const {
    cout << "Question: " << GetQuestion() << endl;
    cout << "Answer Choices:" << endl;
    for (int i=0; i < num_choices_; i++) {
        cout << choices_[i];
        if (show_answers)
            cout << (answers_[i] ? " - correct" : " - incorrect");
        cout << endl;
    }
}

}  // namespace csce240_program5
