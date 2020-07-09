#include "PayOffLookbackCallFixed.h"
#include <algorithm>

PayOffLookbackCallFixed::PayOffLookbackCallFixed(double Strike_) :strike(Strike_)
{
}

double PayOffLookbackCallFixed::operator()(std::vector<double> spotPath) const
{
    double maxSpot = *max_element(spotPath.begin(), spotPath.end());
    return std::max(maxSpot - strike, 0.0);
}

PayOffLookbackCallFixed* PayOffLookbackCallFixed::clone() const
{
    return new PayOffLookbackCallFixed(*this);
}
