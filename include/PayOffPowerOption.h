#pragma once
#define POWEROPTION_H
#include "..\include\PayOff.h"

class PayOffPowerOption : public PayOff
{
public:
	PayOffPowerOption(double Strike_, double Power_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffPowerOption() {}
	virtual PayOffPowerOption* clone() const;
private:
	double Strike;
	double Power;
};