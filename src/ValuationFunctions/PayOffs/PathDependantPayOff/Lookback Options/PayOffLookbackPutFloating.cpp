#include "PayOffLookbackPutFloating.h"
#include <algorithm>
PayOffLookbackPutFloating::PayOffLookbackPutFloating()
{
}

double PayOffLookbackPutFloating::operator()(std::vector<double> spotPath) const
{
    double strike = *max_element(spotPath.begin(), spotPath.end());
    double terminalSpot = spotPath[spotPath.size() - 1];
    return std::max(strike - terminalSpot, 0.0);
}

PayOffLookbackPutFloating* PayOffLookbackPutFloating::clone() const
{
    return new PayOffLookbackPutFloating(*this);
}
