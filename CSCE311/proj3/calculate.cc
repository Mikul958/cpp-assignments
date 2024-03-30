// Copyright 2024 Michael Pikula

#include <proj3/calculate.h>

const char bad_order[] = "Invalid expression, verify the character order.";
const char bad_operator[] = "Invalid operator, please use only +, -, x, or /.";

double Calculate(vector<string> input) {
    // Initialize vectors for operands/operators.
    vector<double> operands;
    vector<string> operators;

    // Evaluate * and / and store remaining work in operands/operators.
    PopulateVectors(input, &operands, &operators);

    // Evaluate remaining addition and subtraction and return result.
    return EvaluateVectors(operands, operators);
}

void PopulateVectors(vector<string> input, vector<double>* operands,
                    vector<string>* operators) {
    bool need_operand = true;  // Alternates to make sure args are in order
    for (string item : input) {
        try {
            double operand = std::stod(item);

            // If last arg was also an operand, exit
            if (!need_operand)
                throw invalid_argument(bad_order);

            // If last operator was * or /, evaluate immediately,
            // else push operand onto stack.
            if (operators->empty()) {
                operands->push_back(operand);
            } else if (operators->back() == "x" || operators->back() == "X") {
                operands->back() = operands->back() * operand;
                operators->pop_back();
            } else if (operators->back() == "/") {
                operands->back() = operands->back() / operand;
                operators->pop_back();
            } else {
                operands->push_back(operand);
            }

            // Require next arg to be an operator.
            need_operand = false;
        }
        // Arg is not a numerical value, must be operator.
        catch(const invalid_argument& e) {
            // If last arg was also an operator, exit
            if (need_operand)
                throw invalid_argument(bad_order);

            // Check validity of operators.
            if (isValidOperator(item))
                operators->push_back(item);
            else
                throw invalid_argument(bad_operator);

            // Require next arg to be an operand.
            need_operand = true;
        }
    }
}

double EvaluateVectors(vector<double> operands, vector<string> operators) {
    double result = operands.front();
    operands.erase(operands.begin());
    while (!operators.empty()) {
        if (operators.front() == "+")
            result = result + operands.front();
        else if (operators.front() == "-")
            result = result - operands.front();
        operands.erase(operands.begin());
        operators.erase(operators.begin());
    }
    return result;
}

bool isValidOperator(string input) {
    return input == "+" ||
           input == "-" ||
           input == "x" ||
           input == "X" ||
           input == "/";
}
