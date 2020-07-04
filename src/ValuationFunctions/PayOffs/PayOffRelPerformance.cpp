#include "PayOffRelPerformance.h"

PayOffRelPerformance::PayOffRelPerformance(double startValue_) : startValue(startValue_)
{
}

double PayOffRelPerformance::operator()(double spot) const
{
    return spot / startValue;
}

PayOffRelPerformance* PayOffRelPerformance::clone() const
{
    return new PayOffRelPerformance(*this);
}
