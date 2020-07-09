#include "PayOffPutGeometricMean.h"
#include <algorithm>
PayOffPutGeometricMean::PayOffPutGeometricMean(double strike_) : strike(strike_)
{
}

double PayOffPutGeometricMean::operator()(std::vector<double> spotPath) const
{
    double product = 1;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        product *= spotPath[i];
    }
    return std::max(strike - pow(product, (1.0 / spotPath.size())), 0.0);
}

PayOffPutGeometricMean* PayOffPutGeometricMean::clone() const
{
    return new PayOffPutGeometricMean(*this);
}
