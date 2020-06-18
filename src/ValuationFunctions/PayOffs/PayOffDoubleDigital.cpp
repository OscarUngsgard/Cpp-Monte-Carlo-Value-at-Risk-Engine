#include "..\include\PayOffDoubleDigital.h"
PayOffDoubleDigital::PayOffDoubleDigital(double Strike1_, double Strike2_)
										: Strike1(Strike1_), Strike2(Strike2_)
{
}

double PayOffDoubleDigital::operator () (double Spot) const
{
	if (Spot <= Strike1)
		return 0;
	if (Spot >= Strike2)
		return 0;
	return 1;
}

PayOffDoubleDigital* PayOffDoubleDigital::clone() const
{
	return new PayOffDoubleDigital(*this);
}
