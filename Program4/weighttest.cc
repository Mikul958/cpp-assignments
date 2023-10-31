#include "weight.h"
#include "weight.cc"

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;

int main()
{
    Weight w1(10, "pounds");
    cout << w1 << endl;

    Weight w2(4535.9237, "grams");
    cout << w2 << endl;

    string result = (w2 > w1) ? "greater" : "not greater";
    cout << "w2 is " << result << " than w1";

    return 0;
}