#include "..\include\PayOffForward.h"

PayOffForward::PayOffForward(double Strike_) : Strike(Strike_)
{
}
double PayOffForward::operator () (double Spot) const
{
	return Spot - Strike;
}

PayOff* PayOffForward::clone() const
{
	return new PayOffForward(*this);
}