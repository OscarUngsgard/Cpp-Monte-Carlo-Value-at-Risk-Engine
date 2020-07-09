#pragma once
#include "PathDependantPayOff.h"
#define PAYOFF_PUT_STRIKE_GEOMETRIC_MEAN_H

class PayOffPutStrikeGeometricMean : public PathDependantPayOff
{
public:
	PayOffPutStrikeGeometricMean();
	virtual double operator()(std::vector<double> spotPath) const;
	virtual ~PayOffPutStrikeGeometricMean() {}
	virtual PayOffPutStrikeGeometricMean* clone() const;
private:
};