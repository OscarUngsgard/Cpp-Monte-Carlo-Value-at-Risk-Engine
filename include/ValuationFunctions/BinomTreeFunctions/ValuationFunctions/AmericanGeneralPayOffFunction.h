#pragma once
#define AMERICAN_GENERAL_PAYOFF_FUNCTION_H
#include "valuationFunction.h"
#include "PayOff.h"
#include "Wrapper.h"

class AmericanGeneralPayOffFunction : public valuationFunction
{
public:
	AmericanGeneralPayOffFunction(std::string uniqueIdentifier_, int nominal_, double S, double r_, double d_, double impvol_, double TTM_, Wrapper<PayOff> thePayOff_, unsigned long binomTreeSteps_);
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
	unsigned long binomTreeSteps;
	Wrapper<PayOff> thePayOff;
};