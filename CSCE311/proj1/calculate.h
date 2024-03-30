// Copyright 2024 Michael Pikula

#ifndef PROJ1_CALCULATE_H_
#define PROJ1_CALCULATE_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::cout;
using std::exception;
using std::invalid_argument;

using std::string;
using std::vector;

/**
 * Takes in a vector of equation arguments and calculates the result.
 * Supports addition, subtraction, multiplication, and division.
 */
double Calculate(vector<string>);

/**
 * Takes in a vector of equation arguments, evaluates multiplication and
 * division, and populates the corresponding output vectors with the remaining
 * operands/operators for addition and subtraction.
 * @exception Throws std::invalid argument upon invalid operator or order.
 */
void PopulateVectors(vector<string> input, vector<double> * operands,
                                           vector<string> * operators);

/**
 * Takes in vectors of operands and operators and evaluates the remaining
 * addition and subtraction.
 */
double EvaluateVectors(vector<double> operands, vector<string> operators);

// Checks if the entered operator is accounted for in this program.
bool isValidOperator(string input);

#endif  // PROJ1_CALCULATE_H_
