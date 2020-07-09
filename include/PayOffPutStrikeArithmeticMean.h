#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_PUT_STRIKE_ARITHMETIC_MEAN_H

class PayOffPutStrikeArithmeticMean : public PathDependantPayOff
{
public:
	PayOffPutStrikeArithmeticMean();
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffPutStrikeArithmeticMean() {}
	virtual PayOffPutStrikeArithmeticMean* clone() const;
private:
};