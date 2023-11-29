// Copyright 2023 mpikula
// A multiple choice question, derived from Question base class.

#ifndef _MULTIPLE_CHOICE_QUESTION_H_
#define _MULTIPLE_CHOICE_QUESTION_H_

#include "question.h"
#include <string>
using std::string;

namespace csce240_program5
{

class MultipleChoiceQuestion : public Question
{
    public:
        explicit MultipleChoiceQuestion(string = "?", int = 0, string * = nullptr, bool * = nullptr);

        // Copy Constructor
        MultipleChoiceQuestion(const MultipleChoiceQuestion&);
        // Overload assignment operator
        MultipleChoiceQuestion& operator = (const MultipleChoiceQuestion&);
        // Destructor
        ~MultipleChoiceQuestion();

        int GetNumChoices() const;
        void SetAnswerChoices(int, string *, bool *);

        void Print(bool) const;

    private:
        int num_choices_;
        string * choices_;
        bool * answers_;

};

}  // namespace csce240_program5

#endif  // _MULTIPLE_CHOICE_QUESTION_H_