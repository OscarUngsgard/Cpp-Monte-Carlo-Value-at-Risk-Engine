#pragma once
#include "PayOff.h"
#define PAYOFF_CALL_RELATIVE_H

class PayOffCallRelative : public PayOff
{
public:
	PayOffCallRelative(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffCallRelative() {}
	virtual PayOffCallRelative* clone() const;
private:
	double Strike;
};