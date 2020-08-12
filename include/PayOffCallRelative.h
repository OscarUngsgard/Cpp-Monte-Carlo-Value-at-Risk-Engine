#pragma once
#include "PayOff.h"
#include "BarrierOptionTypes.h"
#define PAYOFF_CALL_RELATIVE_H

class PayOffCallRelative : public PayOff
{
public:
	PayOffCallRelative(double Strike_, double barrier_ = 1, BarrierOptionType barrierType_ = BarrierOptionType::None);
	virtual double operator()(double Spot) const;
	virtual ~PayOffCallRelative() {}
	virtual PayOffCallRelative* clone() const;
private:
	double Strike;
	BarrierOptionType barrierType; //barrier considered at maturity only. Given as a fraction of Strike.
	double barrier; 
};