// Copyright 2023 mpikula

#include "weightrange.h"

WeightRange::WeightRange()
{
    smallest_ = Weight(0, "pounds");
    largest_ = Weight(0, "pounds");
}
WeightRange::WeightRange(const Weight smallest, const Weight largest)
{
    if (smallest > largest)
    {
        smallest_ = largest;
        largest_ = smallest;
    }
    else
    {
        smallest_ = smallest;
        largest_ = largest;
    }
}

Weight WeightRange::GetSmallest()
{
    return smallest_;
}
Weight WeightRange::GetLargest()
{
    return largest_;
}

void WeightRange::SetSmallest(Weight smallest)
{
    if (smallest > largest_)
        return;
    smallest_ = smallest;
}
void WeightRange::SetLargest(Weight largest)
{
    if (largest < smallest_)
        return;
    largest_ = largest;
}

bool WeightRange::InRange(Weight w, bool inclusive)
{
    if (!inclusive)
        return w > smallest_ && w < largest_;

    // w >= smallest_ && w <= largest_
    return !(w < smallest_) && !(w > largest_);
}
Weight WeightRange::Width()
{
    // Convert a copy of smallest to units of largest.
    Weight smallest_copy(smallest_);
    string largest_units = largest_.GetUnits();
    smallest_copy.ConvertUnits(largest_units);

    // Subtract values to get width and create Weight with that value and units.
    double width_value = largest_.GetValue() - smallest_copy.GetValue();
    Weight width(width_value, largest_units);
    return width;
}