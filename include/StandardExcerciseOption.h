#pragma once
#define STANDARD_EXCERCISE_OPTION_H
#include "PayOff.h"
#include "Wrapper.h"
class StandardExcerciseOption
{
public:
	StandardExcerciseOption(const Wrapper<PayOff>& ThePayOff_, double Expiry_);
	double GetExpiry() const;
	double OptionPayOff(double Spot) const;
private:
	double Expiry;
	Wrapper<PayOff> ThePayOff;
};