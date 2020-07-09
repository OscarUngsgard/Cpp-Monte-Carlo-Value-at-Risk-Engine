#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_LOOKBACK_PUT_FIXED_H

class PayOffLookbackPutFixed : public PathDependantPayOff
{
public:
	PayOffLookbackPutFixed(double Strike_);
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffLookbackPutFixed() {}
	virtual PayOffLookbackPutFixed* clone() const;
private:
	double strike;
};