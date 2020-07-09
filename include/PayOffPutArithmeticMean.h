#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_PUT_ARITHMETIC_MEAN_H

class PayOffPutArithmeticMean : public PathDependantPayOff
{
public:
	PayOffPutArithmeticMean(double Strike_);
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffPutArithmeticMean() {}
	virtual PayOffPutArithmeticMean* clone() const;
private:
	double strike;
};