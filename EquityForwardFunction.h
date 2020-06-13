#pragma once
#define EQUITY_FORWARD_FUNCTION
#include "valuationFunction.h"
//If equity forward then d is div yield. If FX forward then d is simply the foreign risk free rate of return
class EquityForwardFunction : public valuationFunction
{
public:
	EquityForwardFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double TTM_, double strike_);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	int nominal;
	double S;
	double r;
	double d;
	double strike;
};