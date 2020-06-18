#include "PayOffCall.h"
#include <algorithm>

PayOffCall::PayOffCall(double Strike_) : Strike(Strike_)
{
}
double PayOffCall::operator () (double Spot) const
{
	return std::max(Spot - Strike, 0.0);
}
PayOffCall* PayOffCall::clone() const
{
	return new PayOffCall(*this);
}
