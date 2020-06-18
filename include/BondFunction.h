#pragma once
#define BOND_FUNCTION
#include "..\include\valuationFunction.h"
#include "..\include\Arrays.h"
class BondFunction : public valuationFunction
{
public:
	BondFunction(std::string uniqueIdentifier_, int nominal, double yield, double faceValue, double couponRate, int couponFreq, double TTM);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	double yield;
	double faceValue;
	double couponRate;
	int couponFreq;
};