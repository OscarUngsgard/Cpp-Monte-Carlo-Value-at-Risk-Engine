#pragma once
#include "PayOff.h"
#include "BarrierOptionTypes.h"
#define PAYOFF_PUT_RELATIVE_H

class PayOffPutRelative : public PayOff
{
public:
	PayOffPutRelative(double Strike_, double barrier_ = 1, BarrierOptionType barrierType_ = BarrierOptionType::None);
	virtual double operator()(double Spot) const;
	virtual ~PayOffPutRelative() {}
	virtual PayOffPutRelative* clone() const;
private:
	double Strike;
	BarrierOptionType barrierType; //barrier considered at maturity only. Given as a fraction of Strike.
	double barrier;
};