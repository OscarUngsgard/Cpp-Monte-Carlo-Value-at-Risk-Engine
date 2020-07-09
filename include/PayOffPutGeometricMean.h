#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_PUT_GEOMETRIC_MEAN_H

class PayOffPutGeometricMean : public PathDependantPayOff
{
public:
	PayOffPutGeometricMean(double Strike_);
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffPutGeometricMean() {}
	virtual PayOffPutGeometricMean* clone() const;
private:
	double strike;
};