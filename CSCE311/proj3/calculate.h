// Copyright 2024 Michael Pikula

#ifndef PROJ3_CALCULATE_H_
#define PROJ3_CALCULATE_H_

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include <string>
#include <vector>
#include <stack>

using std::cout;
using std::exception;
using std::invalid_argument;

using std::string;
using std::vector;
using std::stack;

void PopulateStacks(vector<string>, stack<double>&, stack<string>&);
double EvaluateStacks(stack<double>&, stack<string>&);

double Calculate(vector<string>);

#endif  // PROJ3_CALCULATE_H_
