#pragma once
#define PAYOFF_DOUBLEDIGITAL_H
#include "PayOff.h"

class PayOffDoubleDigital : public PayOff
{
public:
	PayOffDoubleDigital(double Strike1_, double Strike2_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffDoubleDigital() {}
	virtual PayOffDoubleDigital* clone() const;
private:
	double Strike1;
	double Strike2;
};