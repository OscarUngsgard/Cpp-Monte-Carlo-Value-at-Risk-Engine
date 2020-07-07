#pragma once
#define MONTE_CARLO_PATH_DEPENDANT_OPTION_FUNCTION_H
#include "valuationFunction.h"
#include "PathDependantPayOff.h"
#include "Wrapper.h"
class MonteCarloPathDependantOptionFunction : public valuationFunction
{
public:
	MonteCarloPathDependantOptionFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, const Wrapper<PathDependantPayOff>& ThePayOff_, unsigned long numberOfPaths_, unsigned long numberOfSteps_);
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
	Wrapper<PathDependantPayOff> ThePayOff;
	unsigned long numberOfPaths;
	unsigned long numberOfSteps;
};