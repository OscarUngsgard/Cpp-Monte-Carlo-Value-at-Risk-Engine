#include "PayOffPutStrikeArithmeticMean.h"
#include <algorithm>

PayOffPutStrikeArithmeticMean::PayOffPutStrikeArithmeticMean()
{
}

double PayOffPutStrikeArithmeticMean::operator()(std::vector<double> spotPath) const
{
    double sum = 0;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        sum += spotPath[i];
    }
    double strike = sum / spotPath.size();
    return std::max(strike - spotPath[spotPath.size() - 1], 0.0);
}

PayOffPutStrikeArithmeticMean* PayOffPutStrikeArithmeticMean::clone() const
{
    return new PayOffPutStrikeArithmeticMean(*this);
}
