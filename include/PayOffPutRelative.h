#pragma once
#include "PayOff.h"
#define PAYOFF_PUT_RELATIVE_H

class PayOffPutRelative : public PayOff
{
public:
	PayOffPutRelative(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffPutRelative() {}
	virtual PayOffPutRelative* clone() const;
private:
	double Strike;
};