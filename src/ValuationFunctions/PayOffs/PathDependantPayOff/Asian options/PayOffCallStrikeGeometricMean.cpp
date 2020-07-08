#include "PayOffCallStrikeGeometricMean.h"
#include <algorithm>

PayOffCallStrikeGeometricMean::PayOffCallStrikeGeometricMean()
{
}

double PayOffCallStrikeGeometricMean::operator()(std::vector<double> spotPath) const
{
    double product = 1;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        product *= spotPath[i];
    }
    double strike = pow(product, (1.0 / spotPath.size()));
    return std::max(spotPath[spotPath.size() - 1] - strike, 0.0);
}

PayOffCallStrikeGeometricMean* PayOffCallStrikeGeometricMean::clone() const
{
    return new PayOffCallStrikeGeometricMean(*this);
}
