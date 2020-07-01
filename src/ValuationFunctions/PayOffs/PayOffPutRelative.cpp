#include "PayOffPutRelative.h"
#include <algorithm>

PayOffPutRelative::PayOffPutRelative(double Strike_) : Strike(Strike_)
{
}
double PayOffPutRelative::operator () (double Spot) const
{
	return std::max(1.0 - Spot / Strike, 0.0);
}
PayOffPutRelative* PayOffPutRelative::clone() const
{
	return new PayOffPutRelative(*this);
}
