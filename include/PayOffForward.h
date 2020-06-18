#pragma once
#include "..\include\PayOff.h"
#define PAYOFF_FORWARD_H

class PayOffForward : public PayOff
{
public:
	PayOffForward(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffForward() {}
	virtual PayOff* clone() const;
private:
	double Strike;
};