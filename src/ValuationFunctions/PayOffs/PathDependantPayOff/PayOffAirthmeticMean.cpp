#include "PayOffAirthmeticMean.h"
#include <algorithm>

PayOffArithmeticMean::PayOffArithmeticMean(double Strike_) : strike(Strike_)
{
}

double PayOffArithmeticMean::operator()(std::vector<double> spotPath) const
{
    double sum = 0;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        sum += spotPath[i];
    }
    return std::max( ( sum / (spotPath.size() + 1) ) - strike, 0.0);
}

PayOffArithmeticMean* PayOffArithmeticMean::clone() const
{
    return new PayOffArithmeticMean(*this);
}

