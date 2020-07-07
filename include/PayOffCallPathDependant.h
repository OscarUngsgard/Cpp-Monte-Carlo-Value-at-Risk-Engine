#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_PATH_DEPENDANT_CALL_H
////For debugging the disctrtized Monte Carlo Valuation Engine
class PayOffCallPathDependant : public PathDependantPayOff
{
public:
	PayOffCallPathDependant(double Strike_);
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffCallPathDependant() {}
	virtual PayOffCallPathDependant* clone() const;
private:
	double strike;
};