#include "TreeEuropean.h"

TreeEuropean::TreeEuropean(double FinalTime, const Wrapper<PayOff>& ThePayOff_) : TreeProduct(FinalTime), ThePayOff(ThePayOff_)
{
}

TreeProduct* TreeEuropean::clone() const
{
	return new TreeEuropean(*this);
}

double TreeEuropean::FinalPayOff(double Spot) const
{
	return ThePayOff->operator()(Spot);
}

double TreeEuropean::PreFinalValue(double Spot, double Time, double DiscountedFutureValue) const
{
	return DiscountedFutureValue;
}
