#include "PayOffLookbackCallFloating.h"
#include <algorithm>

PayOffLookbackCallFloating::PayOffLookbackCallFloating()
{
}

double PayOffLookbackCallFloating::operator()(std::vector<double> spotPath) const
{
    double strike = *min_element(spotPath.begin(), spotPath.end());
    double terminalSpot = spotPath[spotPath.size() - 1];
    return std::max(terminalSpot - strike, 0.0);
}

PayOffLookbackCallFloating* PayOffLookbackCallFloating::clone() const
{
    return new PayOffLookbackCallFloating(*this);
}
