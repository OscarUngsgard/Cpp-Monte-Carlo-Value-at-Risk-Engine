#include "PowerOption.h"
#include <minmax.h>
#include <cmath>
PayOffPowerOption::PayOffPowerOption(double Strike_, double Power_) : Strike(Strike_), Power(Power_)
{
}

double PayOffPowerOption::operator()(double Spot) const
{
	return max(pow(Spot,Power),0);
}

PayOffPowerOption* PayOffPowerOption::clone() const
{
	return new PayOffPowerOption(*this);
}


