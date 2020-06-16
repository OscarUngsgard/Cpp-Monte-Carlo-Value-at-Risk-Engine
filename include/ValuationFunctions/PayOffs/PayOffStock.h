#pragma once
#include "PayOff.h"
#define PAYOFF_STOCK_H
class PayOffStock : public PayOff
{
public:
	PayOffStock();
	virtual double operator()(double Spot) const;
	virtual ~PayOffStock() {}
	virtual PayOffStock* clone() const;
private:
};