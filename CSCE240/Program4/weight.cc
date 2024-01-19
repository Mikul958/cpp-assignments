// Copyright 2023 mpikula

#include "weight.h"

// Overload stream insertion operator
ostream& operator << (ostream& whereto, const Weight& w) {
    whereto << w.value_ << " " << w.units_;
    return whereto;
}

// Constructor
Weight::Weight(double value, string units) {
    SetValue(value);
    SetUnits(units);
}

// Accessors
double Weight::GetValue() const {
    return value_;
}
string Weight::GetUnits() const {
    return units_;
}

// Mutators
void Weight::SetValue(double value) {
    if (value >= 0)
        value_ = value;
    else
        value_ = 0;
}
void Weight::SetUnits(string units) {
    if (units == "ounces" || units == "pounds" ||
            units == "grams" || units == "kilograms")
        units_ = units;
    else
        units_ = "pounds";
}

// Misc functions
void Weight::ConvertUnits(string to_units) {
    // 1 lb = 453.59237 g
    const double kPoundsToGrams = 453.59237;

    if (units_ == "ounces") {
        if (to_units == "pounds")
            value_ = value_ / 16;
        else if (to_units == "grams")
            value_ = (value_ / 16) * kPoundsToGrams;
        else if (to_units == "kilograms")
            value_ = (value_ / 16) * kPoundsToGrams / 1000;
    } else if (units_ == "pounds") {
        if (to_units == "ounces")
            value_ = value_ * 16;
        else if (to_units == "grams")
            value_ = value_ * kPoundsToGrams;
        else if (to_units == "kilograms")
            value_ = value_ * kPoundsToGrams / 1000;
    } else if (units_ == "grams") {
        if (to_units == "ounces")
            value_ = value_ / kPoundsToGrams * 16;
        else if (to_units == "pounds")
            value_ = value_ / kPoundsToGrams;
        else if (to_units == "kilograms")
            value_ = value_ / 1000;
    } else if (units_ == "kilograms") {
        if (to_units == "ounces")
            value_ = (value_ * 1000) / kPoundsToGrams * 16;
        else if (to_units == "pounds")
            value_ = (value_ * 1000) / kPoundsToGrams;
        else if (to_units == "grams")
            value_ = value_ * 1000;
    }

    // Checking unit separately so code blocks above aren't huge.
    if (to_units == "ounces" || to_units == "pounds" ||
            to_units == "grams" || to_units == "kilograms")
        units_ = to_units;
}

bool Weight::operator > (const Weight &right) const {
    if (units_ == right.units_)
        return value_ > right.value_;

    Weight right_copy(right.value_, right.units_);
    right_copy.ConvertUnits(units_);
    return value_ > right_copy.value_;
}
bool Weight::operator < (const Weight &right) const {
    if (units_ == right.units_)
        return value_ < right.value_;

    Weight right_copy(right.value_, right.units_);
    right_copy.ConvertUnits(units_);
    return value_ < right_copy.value_;
}
