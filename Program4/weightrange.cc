// Copyright 2023 mpikula

#include "weightrange.h"

WeightRange::WeightRange()
{

}
WeightRange::WeightRange(const Weight smallest, const Weight largest)
{

}

Weight WeightRange::GetLow()
{
    return smallest_;
}
Weight WeightRange::GetHigh()
{
    return largest_;
}

void WeightRange::SetLow(Weight smallest)
{

}
void WeightRange::SetHigh(Weight largest)
{

}

bool WeightRange::InRange(Weight w, bool inclusive)
{

}
Weight WeightRange::Width()
{
    
}