#include "PayOffPowerOption.h"
#include <algorithm>
#include <cmath>
PayOffPowerOption::PayOffPowerOption(double Strike_, double Power_) : Strike(Strike_), Power(Power_)
{
}

double PayOffPowerOption::operator()(double Spot) const
{
	return std::max(pow(Spot,Power) - Strike ,0.0);
}

PayOffPowerOption* PayOffPowerOption::clone() const
{
	return new PayOffPowerOption(*this);
}


