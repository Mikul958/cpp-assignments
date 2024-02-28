# Program 1 - Command line calculator

main.cc - Entry point for the program. Takes in arguments from the command line and passes them into a helper function, then prints the result of the calculation.

calculate.h - Header file containing function prototypes for the calculator.

calculate.cc - Source file containing function implementations for the calculator.
- Calculate: takes in command line arguments and stores them in a vector. This function calls the helper functions PopulateStacks and EvaluateStacks.
- PopulateStacks: takes in a vector containing arguments, as well as pointers to two stacks. Populates the operands and operators stacks, and takes care of multiplication or division along the way if necessary to preserve the order of operations.
- EvaluateStacks: takes in now-populated operands and operators stacks and evaluates their contents until empty, then returns the result.