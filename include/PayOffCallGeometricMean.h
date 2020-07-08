#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_GEOMETRIC_MEAN_H

class PayOffCallGeometricMean : public PathDependantPayOff
{
public:
	PayOffCallGeometricMean(double Strike_);
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffCallGeometricMean() {}
	virtual PayOffCallGeometricMean* clone() const;
private:
	double strike;
};