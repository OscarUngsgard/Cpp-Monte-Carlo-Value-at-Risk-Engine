#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_CALL_H

class PayOffCallArithmeticMean : public PathDependantPayOff
{
public:
	PayOffCallArithmeticMean(double Strike_);
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffCallArithmeticMean() {}
	virtual PayOffCallArithmeticMean* clone() const;
private:
	double strike;
};