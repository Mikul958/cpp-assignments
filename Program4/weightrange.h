// Copyright 2023 mpikula

#ifndef _WEIGHTRANGE_H_
#define _WEIGHTRANGE_H_

#include "weight.h"
#include "weight.cc"  // For testing while still on VS Code

class WeightRange
{
    public:
        explicit WeightRange();
        explicit WeightRange(const Weight smallest, const Weight largest);

        Weight GetLow();
        Weight GetHigh();

        void SetLow(Weight smallest);
        void SetHigh(Weight largest);

        bool InRange(Weight w, bool inclusive = true);
        Weight Width();

    private:
        Weight smallest_;
        Weight largest_;
};

#endif  // _WEIGHTRANGE_H_