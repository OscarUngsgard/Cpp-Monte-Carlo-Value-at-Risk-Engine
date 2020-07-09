#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_LOOKBACK_PUT_FLOATING_H

class PayOffLookbackPutFloating : public PathDependantPayOff
{
public:
	PayOffLookbackPutFloating();
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffLookbackPutFloating() {}
	virtual PayOffLookbackPutFloating* clone() const;
private:
};