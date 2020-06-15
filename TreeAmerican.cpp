#include "TreeAmerican.h"
#include <algorithm>

TreeAmerican::TreeAmerican(double FinalTime_, const Wrapper<PayOff>& ThePayOff_) : TreeProduct(FinalTime_), ThePayOff(ThePayOff_)
{
}

TreeProduct* TreeAmerican::clone() const
{
	return new TreeAmerican(*this);
}

double TreeAmerican::FinalPayOff(double Spot) const
{
	return ThePayOff->operator()(Spot);
}

double TreeAmerican::PreFinalValue(double Spot, double Time, double DiscountedFutureValue) const
{
	return std::max(ThePayOff->operator()(Spot), DiscountedFutureValue);
}
