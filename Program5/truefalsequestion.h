// Copyright 2023 mpikula
// A true or false question, derived from Question base class.

#ifndef _TRUE_FALSE_QUESTION_H_
#define _TRUE_FALSE_QUESTION_H_

#include <string>
using std::string;
#include "question.h"

namespace csce240_program5 {

class TrueFalseQuestion : public Question {
 public:
    explicit TrueFalseQuestion(string = "?", bool = true);

    bool GetAnswer() const;
    void SetAnswer(bool);

    void Print(bool) const;

 private:
    bool answer_;
};

}  // namespace csce240_program5

#endif  // _TRUE_FALSE_QUESTION_H_
