#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_LOOKBACK_CALL_FLOATING_H

class PayOffLookbackCallFloating : public PathDependantPayOff
{
public:
	PayOffLookbackCallFloating();
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffLookbackCallFloating() {}
	virtual PayOffLookbackCallFloating* clone() const;
private:
};