#include "Weight.h"
#include "Weight.cc"
#include <iomanip>

int main()
{
    cout << std::setprecision(15);

    Weight w1(2, "pounds");
    cout << w1 << endl;  // 2 pounds
    w1.ConvertUnits("ounces");
    cout << w1 << endl;  // 32 ounces

    cout << endl;

    Weight w2(8, "ounces");
    cout << w2 << endl;  // 8 ounces
    w2.ConvertUnits("pounds");
    cout << w2 << endl;  // 0.5 pounds

    cout << endl;

    Weight w3(4, "pounds");
    cout << w3 << endl;  // 4 pounds
    w3.ConvertUnits("grams");
    cout << w3 << endl;  // approx. 1814.36948 grams

    cout << endl;

    Weight w4(1814.36948, "grams");
    cout << w4 << endl;  // 1814.36948 grams
    w4.ConvertUnits("pounds");
    cout << w4 << endl;  // approx. 4 pounds

    return 0;
}