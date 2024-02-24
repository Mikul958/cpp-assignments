// Copyright 2024 mpikula

#ifndef PROJ2_CALCULATE_H_
#define PROJ2_CALCULATE_H_

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <stack>
using std::stack;

void PopulateStacks(vector<string>, stack<double>&, stack<string>&);
double EvaluateStacks(stack<double>&, stack<string>&);

double Calculate(vector<string>);

#endif  // PROJ2_CALCULATE_H_
