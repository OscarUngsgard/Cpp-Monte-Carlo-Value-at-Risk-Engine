#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_LOOKBACK_CALL_FIXED_H

class PayOffLookbackCallFixed : public PathDependantPayOff
{
public:
	PayOffLookbackCallFixed(double Strike_);
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffLookbackCallFixed() {}
	virtual PayOffLookbackCallFixed* clone() const;
private:
	double strike;
};