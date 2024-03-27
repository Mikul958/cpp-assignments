// Copyright 2024 Michael Pikula

#ifndef PROJ3_CALCULATE_H_
#define PROJ3_CALCULATE_H_

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <stack>
using std::stack;

void PopulateStacks(vector<string>, stack<double>&, stack<string>&);
double EvaluateStacks(stack<double>&, stack<string>&);

double Calculate(vector<string>);

#endif  // PROJ3_CALCULATE_H_
