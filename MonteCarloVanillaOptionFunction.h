#pragma once
#define MONTE_CARLO_VANILLA_OPTION_FUNCTION_H
#include "valuationFunction.h"
#include "PayOff.h"

#include "Wrapper.h"
class MonteCarloVanillaOptionFunction : public valuationFunction
{
public:
	MonteCarloVanillaOptionFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, const Wrapper<PayOff>& ThePayOff_, unsigned long numberOfPaths_);
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
	double impvol;
	Wrapper<PayOff> ThePayOff;
	unsigned long numberOfPaths;
};