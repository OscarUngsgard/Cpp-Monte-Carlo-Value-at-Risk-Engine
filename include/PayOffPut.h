#pragma once
#include "..\include\PayOff.h"
#define PAYOFF_PUT_H

class PayOffPut : public PayOff
{
public:
	PayOffPut(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffPut() {}
	virtual PayOffPut* clone() const;
private:
	double Strike;
};