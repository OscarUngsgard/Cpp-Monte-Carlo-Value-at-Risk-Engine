#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_STRIKE_ARITHMETIC_MEAN_H

class PayOffCallStrikeArithmeticMean : public PathDependantPayOff
{
public:
	PayOffCallStrikeArithmeticMean();
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffCallStrikeArithmeticMean() {}
	virtual PayOffCallStrikeArithmeticMean* clone() const;
private:
};