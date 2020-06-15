#pragma once
#define FRA_FUNCTION
#include "valuationFunction.h"
#include "Arrays.h"
class FRAFunction : public valuationFunction
{
public:
	FRAFunction(std::string uniqueIdentifier_, int nominal, double r_, double contractRate, double forwardRate, double TimeToForwardtime, double TTM);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	void UpdateTTM(double timeStep);
	virtual valuationFunction* clone() const;
private:
	double r;
	double contractRate;
	double forwardRate;
	double TimeToForwardtime;
};