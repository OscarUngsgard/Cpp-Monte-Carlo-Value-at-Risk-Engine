#include "PayOffPutArithmeticMean.h"
#include <algorithm>

PayOffPutArithmeticMean::PayOffPutArithmeticMean(double Strike_) : strike(Strike_)
{
}

double PayOffPutArithmeticMean::operator()(std::vector<double> spotPath) const
{
    double sum = 0;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        sum += spotPath[i];
    }
    return std::max(strike - sum / spotPath.size(), 0.0);
}

PayOffPutArithmeticMean* PayOffPutArithmeticMean::clone() const
{
    return new PayOffPutArithmeticMean(*this);
}
