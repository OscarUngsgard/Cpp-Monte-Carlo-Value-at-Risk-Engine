#include "PayOffPutRelative.h"
#include <algorithm>

PayOffPutRelative::PayOffPutRelative(double Strike_, double barrier_, BarrierOptionType barrierType_) : Strike(Strike_), barrier(barrier_), barrierType(barrierType_)
{
}
double PayOffPutRelative::operator () (double Spot) const
{
	double thisPayoff = std::max(1.0 - Spot / Strike, 0.0);
	switch (barrierType) //Note: This is not a path dependant barrier, it is only considered at maturity as is standard for e.g. rainbow optoins.
	{
	case BarrierOptionType::downAndOut:
		thisPayoff = ((Spot < (Strike* barrier)) ? 0 : thisPayoff);
		break;
	case BarrierOptionType::downAndIn:
		thisPayoff = ((Spot < (Strike* barrier)) ? thisPayoff : 0);
		break;
	case BarrierOptionType::upAndOut:
		thisPayoff = ((Spot > (Strike * barrier)) ? 0 : thisPayoff);
		break;
	case BarrierOptionType::upAndIn:
		thisPayoff = ((Spot > (Strike * barrier)) ? thisPayoff : 0);
		break;
	case BarrierOptionType::None:
		break;
	default:
		break;
	}
	return thisPayoff;
}
PayOffPutRelative* PayOffPutRelative::clone() const
{
	return new PayOffPutRelative(*this);
}
