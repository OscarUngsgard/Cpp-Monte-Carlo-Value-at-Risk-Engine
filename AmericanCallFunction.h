#pragma once
#define AMERICAN_CALL_FUNCTION_H
#include "valuationFunction.h"
class AmericanCallFunction : public valuationFunction
{
public:
	AmericanCallFunction(std::string uniqueIdentifier_, int nominal_, double S, double r_, double d_, double impvol_, double TTM_, double strike_, unsigned long binomTreeSteps_);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	double S;
	double r;
	double d;
	double impvol;
	double strike;
	unsigned long binomTreeSteps;
};