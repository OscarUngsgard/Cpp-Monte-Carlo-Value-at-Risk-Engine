#pragma once
#define FIXED_FOR_FIXED_FX_SWAP_FUNCTION
#include "valuationFunction.h"
#include "Arrays.h"
class FixedForFixedFXSwapFunction : public valuationFunction
{
public:
	FixedForFixedFXSwapFunction(std::string uniqueIdentifier_, int notional_domestic_, int notional_foreign_, double FXRate_, double r_domestic_, double r_foreign_, double contractRate_domestic, double contractRate_foreign, unsigned long frequency_, double TTM);
	FixedForFixedFXSwapFunction(std::string uniqueIdentifier_, int notional_domestic, int notional_foreign_, double FXRate_, MJArray r_domestic_rates_, MJArray r_foreign_rates_, double contractRate_domestic, double contractRate_foreign, unsigned long frequency_, double TTM);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	int notional_domestic;
	int notional_foreign;
	MJArray discountingRates_domestic;
	MJArray discountingRates_foreign;
	double contractRate_domestic;
	double contractRate_foreign;
	double FXRate;
	unsigned long frequency;
};