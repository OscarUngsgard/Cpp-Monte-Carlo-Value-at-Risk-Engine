#pragma once
#define BSPUTFUNCTION_H
#include "valuationFunction.h"
class BSPutFunction : public valuationFunction
{
public:
	BSPutFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, double strike_);
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
};