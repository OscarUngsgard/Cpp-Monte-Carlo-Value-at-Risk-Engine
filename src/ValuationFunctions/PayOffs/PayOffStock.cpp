#include "PayOffStock.h"

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
