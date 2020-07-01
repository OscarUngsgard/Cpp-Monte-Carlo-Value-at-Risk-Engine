#include "PayOffCallRelative.h"
#include <algorithm>

PayOffCallRelative::PayOffCallRelative(double Strike_) : Strike(Strike_)
{
}
double PayOffCallRelative::operator () (double Spot) const
{
	return std::max(Spot / Strike - 1.0, 0.0);
}
PayOffCallRelative* PayOffCallRelative::clone() const
{
	return new PayOffCallRelative(*this);
}
