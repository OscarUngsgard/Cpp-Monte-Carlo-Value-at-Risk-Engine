#pragma once
#define AMERICAN_PUT_FUNCTION_H
#include "..\include\valuationFunction.h"
//Makes use of the binomial tree code provided in C++ Design Patterns, augmented to use risk neutral probabilities of up and down movements to make the implied discounted price a martingale
class AmericanPutFunction : public valuationFunction
{
public:
	AmericanPutFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, double strike_, unsigned long binomTreeSteps_);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	double r;
	double S;
	double d;
	double impvol;
	double strike;
	unsigned long binomTreeSteps;
};