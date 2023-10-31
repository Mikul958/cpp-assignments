// Copyright 2023 mpikula

#ifndef _WEIGHT_H_
#define _WEIGHT_H_

#include <string>
using std::string;
#include <iostream>
using std::ostream;

class Weight
{
    friend ostream& operator << (ostream& whereto, const Weight& w);

    public:
        explicit Weight(double value = 0, string units = "pounds");

        double GetValue() const;
        string GetUnits() const;

        void SetValue(double value);
        void SetUnits(string units);

        void ConvertUnits(string to_units);

        bool operator > (const Weight &right) const;
        bool operator < (const Weight &right) const;

    private:
        double value_;
        string units_;
};

#endif  // _WEIGHT_H_