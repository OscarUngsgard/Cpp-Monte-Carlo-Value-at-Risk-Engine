#include "..\include\StandardExcerciseOption.h"
StandardExcerciseOption::StandardExcerciseOption(const Wrapper<PayOff>& ThePayOff_, double Expiry_) : ThePayOff(ThePayOff_), Expiry(Expiry_)
{
}
double StandardExcerciseOption::GetExpiry() const
{
	return Expiry;
}
double StandardExcerciseOption::OptionPayOff(double Spot) const
{
	return ThePayOff->operator()(Spot);
}
