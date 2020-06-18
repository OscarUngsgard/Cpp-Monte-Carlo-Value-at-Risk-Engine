#include "..\include\PayOffPut.h"
#include <algorithm>

PayOffPut::PayOffPut(double Strike_) : Strike(Strike_)
{
}
double PayOffPut::operator () (double Spot) const
{
	return std::max(Strike - Spot, 0.0);
}
PayOffPut* PayOffPut::clone() const
{
	return new PayOffPut(*this);
}