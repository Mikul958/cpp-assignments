// Copyright 2023 mpikula

#ifndef _WEIGHTRANGE_H_
#define _WEIGHTRANGE_H_

#include "weight.h"
// #include "weight.cc"  // For testing while still on VS Code

class WeightRange {
 public:
    WeightRange();
    explicit WeightRange(const Weight smallest, const Weight largest);

    Weight GetSmallest();
    Weight GetLargest();

    void SetSmallest(Weight smallest);
    void SetLargest(Weight largest);

    bool InRange(Weight w, bool inclusive = true);
    Weight Width();

 private:
    Weight smallest_;
    Weight largest_;
};

#endif  // _WEIGHTRANGE_H_
