#include "PayOffCallPathDependant.h"
#include <algorithm>
#include <iostream>
PayOffCallPathDependant::PayOffCallPathDependant(double Strike_) : strike(Strike_)
{
}

double PayOffCallPathDependant::operator()(std::vector<double> spotPath) const
{
    return std::max(spotPath[spotPath.size() - 1] - strike, 0.0);
}

PayOffCallPathDependant* PayOffCallPathDependant::clone() const
{
    return new PayOffCallPathDependant(*this);
}
