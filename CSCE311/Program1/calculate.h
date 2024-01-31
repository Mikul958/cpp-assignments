// Copyright 2024 mpikula

#ifndef PROGRAM1_CALCULATE_H_
#define PROGRAM1_CALCULATE_H_

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <stack>
using std::stack;

void ExitInvalidOrder();
void ExitInvalidChar();
void ExitDivideByZero();

void PopulateStacks(vector<string>&, stack<double>&, stack<string>&);
double EvaluateStacks(stack<double>&, stack<string>&);

double Calculate(int, char**);

#endif  // PROGRAM1_CALCULATE_H_
