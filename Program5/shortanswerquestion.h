// Copyright 2023 mpikula
// A short answer question, derived from Question base class.

#ifndef _SHORT_ANSWER_QUESTION_H_
#define _SHORT_ANSWER_QUESTION_H_

#include "question.h"
#include <string>
using std::string;

namespace csce240_program5
{

class ShortAnswerQuestion : public Question
{
    public:
        explicit ShortAnswerQuestion(string = "?", string = "");

        string GetAnswer() const;
        void SetAnswer(string);

        void Print(bool) const;

    private:
        string answer_;
};

}  // namespace csce240_program5

#endif  // _SHORT_ANSWER_QUESTION_H_
