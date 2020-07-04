#pragma once
#include "PayOff.h"
#define PAYOFF_REL_PERFORMANCE_H

class PayOffRelPerformance : public PayOff
{
public:
	PayOffRelPerformance(double startValue_);
	virtual double operator()(double spot) const;
	virtual ~PayOffRelPerformance() {}
	virtual PayOffRelPerformance* clone() const;
private:
	double startValue;
};