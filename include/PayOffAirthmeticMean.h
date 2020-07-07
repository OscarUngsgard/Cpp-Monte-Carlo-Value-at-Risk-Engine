#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_CALL_H

class PayOffArithmeticMean : public PathDependantPayOff
{
public:
	PayOffArithmeticMean(double Strike_);
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffArithmeticMean() {}
	virtual PayOffArithmeticMean* clone() const;
private:
	double strike;
};