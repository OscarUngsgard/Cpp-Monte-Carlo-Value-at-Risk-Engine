#pragma once
#include "..\include\PayOff.h"
#define PAYOFF_CALL_H

class PayOffCall : public PayOff
{
public:
	PayOffCall(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffCall() {}
	virtual PayOffCall* clone() const;
private:
	double Strike;
};