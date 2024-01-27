#include <Program1/calculator.h>

#include <iostream>
using std::cout;
#include <string>
using std::string;
using std::stod;
#include <vector>
using std::vector;
#include <stack>
using std::stack;

void ExitInvalidOrder() {
    cout << "Invalid expression entered, check the order of operands and operators.\n";
    exit(EXIT_FAILURE);
}

void ExitInvalidChar() {
    cout << "Invalid character entered, please only use +, -, x, and /.";
    exit(EXIT_FAILURE);
}

double EvaluateStacks(stack<double> operands, stack<string> operators) {
    double result = operands.top();
    operands.pop();

    // THIS DEFINITELY DOESN'T WORK RIGHT
    while (!operators.empty()) {
        if (operators.top() == "+")
            result = operands.top() + result;
        else if (operators.top() == "-")
            result = operands.top() - result;
        operands.pop();
        operators.pop();
    }

    return result;
}

double Calculate(int argc, char* argv[]) {
    // Cast all args to string and add to vector.
    vector<string> input;
    for (int i=1; i<argc; i++)
        input.push_back(string(argv[i]));
    
    // Set up stacks for operands/operators and add args
    stack<double> operands;
    stack<string> operators;
    bool need_operand = true;  // Alternates to make sure args are in order
    for (string item : input)
    {
        try {
            double current = stod(item);
            // If last arg was an operand and this one is too, exit
            if (!need_operand)
                ExitInvalidOrder();

            // Check last operator to uphold order of operations.
            if (operators.empty()) {
                operands.push(current);
            }
            else if (operators.top() == "x" || operators.top() == "X") {
                operands.top() = operands.top() * current;
                operators.pop();
            }
            else if (operators.top() == "/") {
                operands.top() = operands.top() / current;
                operators.pop();
            }
            else {
                operands.push(current);
            }
            
            need_operand = false;
        }
        catch(const std::invalid_argument& e) {
            string current = item;
            // If last arg was an operator and this one is too, exit
            if (need_operand)
                ExitInvalidOrder();
            
            // Check operators
            // TODO
            if (current == "+" || current == "-" || current == "x" || current == "X" || current == "/")
                operators.push(current);
            else
                ExitInvalidChar();

            need_operand = true;
        }
    }

    return EvaluateStacks(operands, operators);
}