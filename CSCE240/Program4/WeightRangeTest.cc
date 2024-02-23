#include "weightrange.h"
#include "weightrange.cc"  // For testing while still on VS Code.
#include <iostream>
using std::cout;
using std::endl;
int main()
{
    Weight w1(30, "pounds");
    Weight w2(25, "pounds");

    WeightRange range(w1, w2);

    cout << range.GetSmallest() << " -> " << range.GetLargest() << endl;

    // Should change.
    range.SetSmallest(Weight(20, "pounds"));
    cout << range.GetSmallest() << " -> " << range.GetLargest() << endl;

    // Should not change.
    range.SetLargest(Weight(100, "grams"));
    cout << range.GetSmallest() << " -> " << range.GetLargest() << endl;

    // Should change.
    range.SetLargest(Weight(10752, "grams"));
    cout << range.GetSmallest() << " -> " << range.GetLargest() << endl;

    // True
    Weight w3(10, "kilograms");
    cout << range.InRange(w3) << endl;

    // 20 pounds ~= 9071.85 grams
    cout << range.Width() << endl;
}