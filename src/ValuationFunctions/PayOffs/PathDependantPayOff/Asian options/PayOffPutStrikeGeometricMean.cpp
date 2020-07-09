#include "PayOffPutStrikeGeometricMean.h"
#include <algorithm>

PayOffPutStrikeGeometricMean::PayOffPutStrikeGeometricMean()
{
}

double PayOffPutStrikeGeometricMean::operator()(std::vector<double> spotPath) const
{
    double product = 1;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        product *= spotPath[i];
    }
    double strike = pow(product, (1.0 / spotPath.size()));
    return std::max(strike - spotPath[spotPath.size() - 1], 0.0);
}

PayOffPutStrikeGeometricMean* PayOffPutStrikeGeometricMean::clone() const
{
    return new PayOffPutStrikeGeometricMean(*this);
}
