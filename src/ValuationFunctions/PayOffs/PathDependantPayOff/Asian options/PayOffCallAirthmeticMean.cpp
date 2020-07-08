#include "PayOffCallAirthmeticMean.h"
#include <algorithm>

PayOffCallArithmeticMean::PayOffCallArithmeticMean(double Strike_) : strike(Strike_)
{
}

double PayOffCallArithmeticMean::operator()(std::vector<double> spotPath) const
{
    double sum = 0;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        sum += spotPath[i];
    }
    return std::max( sum / spotPath.size()  - strike, 0.0);
}

PayOffCallArithmeticMean* PayOffCallArithmeticMean::clone() const
{
    return new PayOffCallArithmeticMean(*this);
}

