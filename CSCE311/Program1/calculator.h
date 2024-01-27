#ifndef _PROGRAM1_CALCULATOR_H_
#define _PROGRAM1_CALCULATOR_H_

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <stack>
using std::stack;

void ExitInvalidOrder();
void ExitInvalidChar();

void PopulateStacks(vector<string>, stack<double>, stack<string>);
double EvaluateStacks(stack<double>, stack<string>);

double Calculate(int, char**);

#endif  // _PROGRAM1_CALCULATOR_H_