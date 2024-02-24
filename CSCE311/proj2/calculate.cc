// Copyright 2024 mpikula

#include <proj2/calculate.h>

#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <algorithm>
#include <stack>
using std::stack;
#include <stdexcept>
using std::exception;
using std::invalid_argument;

void PopulateStacks(vector<string> input, stack<double>* operands,
                    stack<string>* operators) {
    bool need_operand = true;  // Alternates to make sure args are in order
    for (string item : input) {
        string bad_order = "Invalid expression, verify the character order.";
        string bad_char = "Invalid expression, use only +, -, x, or /.";
        string divide_by_zero = "Invalid expression, contains division by 0.";
        try {
            double current = std::stod(item);
            // If last arg was also an operand, exit
            if (!need_operand)
                throw invalid_argument(bad_order);

            // Check last operator to uphold order of operations.
            if (operators->empty()) {
                operands->push(current);
            } else if (operators->top() == "x" || operators->top() == "X") {
                operands->top() = current * operands->top();
                operators->pop();
            } else if (operators->top() == "/") {
                if (operands->top() == 0)
                    throw invalid_argument(divide_by_zero);
                operands->top() = current / operands->top();
                operators->pop();
            } else {
                operands->push(current);
            }

            need_operand = false;
        }
        // Arg is not a numerical value.
        catch(const invalid_argument& e) {
            string current = item;
            // If last arg was also an operator, exit
            if (need_operand)
                throw invalid_argument(bad_order);

            // Check validity of operators.
            if (current == "+" || current == "-" || current == "x"
            || current == "X" || current == "/")
                operators->push(current);
            else
                throw invalid_argument(bad_char);

            need_operand = true;
        }
    }
}

double EvaluateStacks(stack<double>* operands, stack<string>* operators) {
    double result = operands->top();
    operands->pop();
    while (!operators->empty()) {
        if (operators->top() == "+")
            result = result + operands->top();
        else if (operators->top() == "-")
            result = result - operands->top();
        operands->pop();
        operators->pop();
    }
    return result;
}

double Calculate(vector<string> input) {
    // Set up stacks for operands/operators and add args
    // Note: Args added to stacks are now back in forwards order
    stack<double> operands;
    stack<string> operators;

    // Reverses order of input vector so that stacks evaluate left to right
    std::reverse(input.begin(), input.end());

    PopulateStacks(input, &operands, &operators);
    return EvaluateStacks(&operands, &operators);
}
