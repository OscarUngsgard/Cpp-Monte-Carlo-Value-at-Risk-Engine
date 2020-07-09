#include "PayOffLookbackPutFixed.h"
#include <algorithm>

PayOffLookbackPutFixed::PayOffLookbackPutFixed(double Strike_) : strike(Strike_)
{
}

double PayOffLookbackPutFixed::operator()(std::vector<double> spotPath) const
{
    double minSpot = *min_element(spotPath.begin(), spotPath.end());
    return std::max(strike - minSpot, 0.0);
}

PayOffLookbackPutFixed* PayOffLookbackPutFixed::clone() const
{
    return new PayOffLookbackPutFixed(*this);
}
