#pragma once
#define INTEREST_RATE_SWAP_FUNCTION
#include "..\include\valuationFunction.h"
#include "..\include\Arrays.h"
class InterestRateSwapFunction : public valuationFunction
{
public:
	InterestRateSwapFunction(std::string uniqueIdentifier_, int nominal, double r_, double contractRate, MJArray continousCompoundingForwardRates, unsigned long frequency_, double TTM);
	InterestRateSwapFunction(std::string uniqueIdentifier_, int nominal, MJArray riskFreeRates_, double contractRate, MJArray continousCompoundingForwardRates, unsigned long frequency_, double TTM);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	MJArray continousCompoundingForwardRates;
	MJArray discountingRates;
	double contractRate;
	unsigned long frequency;
};