#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_STRIKE_GEOMETRIC_MEAN_H

class PayOffCallStrikeGeometricMean : public PathDependantPayOff
{
public:
	PayOffCallStrikeGeometricMean();
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffCallStrikeGeometricMean() {}
	virtual PayOffCallStrikeGeometricMean* clone() const;
private:
};