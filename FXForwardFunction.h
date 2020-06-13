#pragma once
#define FX_FORWARD_FUNCTION
#include "valuationFunction.h"
//If equity forward then d is div yield. If FX forward then d is simply the foreign risk free rate of return
class FXForwardFunction : public valuationFunction
{
public:
	FXForwardFunction(std::string uniqueIdentifier_, int nominal_, double X_, double r_domestic_, double r_foreign_, double TTM_, double strike_);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	int nominal;
	double X;
	double r_domestic;
	double r_foreign;
	double strike;
};