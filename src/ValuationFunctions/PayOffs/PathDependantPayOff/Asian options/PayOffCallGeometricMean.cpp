#include "PayOffCallGeometricMean.h"
#include <algorithm>

PayOffCallGeometricMean::PayOffCallGeometricMean(double Strike_) : strike(Strike_)
{
}

double PayOffCallGeometricMean::operator()(std::vector<double> spotPath) const
{
    double product = 1;
    for (unsigned long i = 0; i < spotPath.size(); i++)
    {
        product *= spotPath[i];
    }
    return std::max(pow(product, (1.0 / spotPath.size() ) ) - strike , 0.0);
}

PayOffCallGeometricMean* PayOffCallGeometricMean::clone() const
{
    return new PayOffCallGeometricMean(*this);
}
