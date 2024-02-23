// Copyright 2023 mpikula

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include "program2functions.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

int SumDigits(int number) {
    int remainder, total = 0;

    // Add rightmost digit to total and divide by 10 until no more digits.
    while (number != 0) {
        remainder = number % 10;
        total += remainder;
        number = number / 10;
    }

    // Absolute value
    total = abs(total);
    return total;
}

bool IsPalindrome(int number) {
    int temp = number, remainder, reversed = 0;

    // Add rightmost digit to rightmost place of reversed until no more digits.
    while (temp != 0) {
        remainder = temp % 10;
        reversed = reversed*10 + remainder;
        temp = temp / 10;
    }
    return number == reversed;
}

int CountDigitMatch(int number, int target) {
    // Return -1 if target is not a positive, single digit.
    if (target < 0 || target > 9) {
        return -1;
    }

    number = abs(number);

    int remainder, matches = 0;

    // Compare rightmost digit with target, then discard it.
    while (number != 0) {
        remainder = number % 10;
        if (remainder == target) {
            matches++;
        }
        number = number / 10;
    }
    return matches;
}

// Uses CountDigitMatch() defined above to compare digits.
bool SameDigits(int number_1, int number_2) {
    int temp_1, temp_2;
    for (int i=0; i <= 9; i++) {
        // Count amount of current digit (i) in each number.
        temp_1 = CountDigitMatch(number_1, i);
        temp_2 = CountDigitMatch(number_2, i);

        // As soon as counts are not equal, return false and kick out.
        if (temp_1 != temp_2) {
            return false;
        }
    }
    // If counts for all 10 digits matched, return true.
    return true;
}

void Factor(int number) {
    int temp = number;
    bool prime = true;
    string output;   // String that keeps track of all factors found.

    // If number is negative, add - to output and make it positive.
    if (temp < 0) {
        temp = -temp;
        output += '-';
    }

    // Divide number by 2 and store factors in output until odd.
    while (temp % 2 == 0) {
        // Add * before factor in output string if not first factor.
        if (prime) {
            output += '2';
        } else {
            output += " * 2";
        }
        temp = temp / 2;
        prime = false;
    }

    // Iterate through odd numbers and find all factors for each.
    for (int i=3; i <= sqrt(temp); i+=2) {
        while (temp % i == 0) {
            if (prime) {
                // i must be converted to string before concatenation.
                output += to_string(i);
            } else {
                output += " * " + to_string(i);
            }
            temp = temp / i;
            prime = false;
        }
    }

    // Edge case, in case prime factor is somehow left over.
    if (temp > 1) {
        // No need for branch, if original number is prime output isn't shown.
        output += " * " + to_string(temp);
    }

    // If no prime factors found, say number is prime, else print output.
    if (prime) {
        cout << number << " is prime" << endl;
    } else {
        cout << number << " = " << output << endl;
    }
}

// &parameter: variables used in arguments 2 and 3 are updated by function.
void DoubleMinToIntMinSec(double entry, int &minutes, int &seconds) {
    // Sets integer portion of entry to minutes portion.
    minutes = entry;

    // Take decimal portion, multiply by 60, and round for seconds.
    seconds = round((entry-minutes) * 60);
}
