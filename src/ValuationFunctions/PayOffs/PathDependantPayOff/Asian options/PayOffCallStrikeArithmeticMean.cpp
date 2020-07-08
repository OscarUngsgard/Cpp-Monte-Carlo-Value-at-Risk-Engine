#include "PayOffCallStrikeArithmeticMean.h"
#include <algorithm>

PayOffCallStrikeArithmeticMean::PayOffCallStrikeArithmeticMean()
{
}

double PayOffCallStrikeArithmeticMean::operator()(std::vector<double> spotPath) const
{
    double sum = 0;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        sum += spotPath[i];
    }
    double strike = sum / spotPath.size();
    return std::max(spotPath[spotPath.size()-1] - strike, 0.0);
}

PayOffCallStrikeArithmeticMean* PayOffCallStrikeArithmeticMean::clone() const
{
    return new PayOffCallStrikeArithmeticMean(*this);
}
