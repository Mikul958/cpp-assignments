// Copyright 2023 bhipp
// definition of the Question base class
#ifndef _QUESTION_H_
#define _QUESTION_H_

#include<string>
using std::string;
#include<iostream>

namespace csce240_program5 {

class Question {
 public:
  explicit Question(string q = "?") { question_ = q; }
  void SetQuestion(string q) {
    if ( q.length() > 0 )
      question_ = q;
  }
  string GetQuestion() const { return question_; }
  void Print() const {
    std::cout << "Question: " << question_ << std::endl;
  }
 private:
  string question_;
};

}  // namespace csce240_program5

#endif  // _QUESTION_H_
