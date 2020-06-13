#include "PayOff.h"
#include <minmax.h>

PayOffCall::PayOffCall(double Strike_) : Strike(Strike_)
{
}
double PayOffCall::operator () (double Spot) const
{
	return max(Spot - Strike, 0.0);
}
PayOffCall* PayOffCall::clone() const
{
	return new PayOffCall(*this);
}

PayOffPut::PayOffPut(double Strike_) : Strike(Strike_)
{
}
double PayOffPut::operator () (double Spot) const
{
	return max(Strike - Spot, 0.0);
}
PayOffPut* PayOffPut::clone() const
{
	return new PayOffPut(*this);
}


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


PayOffStock::PayOffStock()
{
}
double PayOffStock::operator()(double Spot) const
{
	return Spot;
}
PayOffStock* PayOffStock::clone() const
{
	return new PayOffStock(*this);
}
