#include "FixedForFixedFXSwapFunction.h"
#include <cmath>
#include <iostream>

FixedForFixedFXSwapFunction::FixedForFixedFXSwapFunction(std::string uniqueIdentifier_, int notional_domestic_, int notional_foreign_, double FXRate_, double r_domestic_, double r_foreign_, double contractRate_domestic_, double contractRate_foreign_ , unsigned long frequency_, double TTM_) : valuationFunction(uniqueIdentifier_, TTM_, 1), notional_domestic(notional_domestic_), notional_foreign(notional_foreign_), FXRate(FXRate_), frequency(frequency_), contractRate_domestic(contractRate_domestic_), contractRate_foreign(contractRate_foreign_)
{
	unsigned long swapCount(ceil(TTM * frequency));
	discountingRates_domestic.resize(swapCount);
	discountingRates_domestic = r_domestic_;

	discountingRates_foreign.resize(swapCount);
	discountingRates_foreign = r_foreign_;
}

FixedForFixedFXSwapFunction::FixedForFixedFXSwapFunction(std::string uniqueIdentifier_, int notional_domestic_, int notional_foreign_, double FXRate_, MJArray r_domestic_rates_, MJArray r_foreign_rates_, double contractRate_domestic_, double contractRate_foreign_, unsigned long frequency_, double TTM_) : valuationFunction(uniqueIdentifier_, TTM_,1), notional_domestic(notional_domestic_), notional_foreign(notional_foreign_), FXRate(FXRate_), discountingRates_domestic(r_domestic_rates_), discountingRates_foreign(r_foreign_rates_), frequency(frequency_), contractRate_domestic(contractRate_domestic_), contractRate_foreign(contractRate_foreign_)
{
	if (r_domestic_rates_.size() != r_foreign_rates_.size())
		throw("mismatched size of domestic and foreign rates");
}

void FixedForFixedFXSwapFunction::ValueInstrument()
{
	MJArray forwardTimes(ceil(TTM * frequency));
	double timeToNextForwardTime = std::fmod(TTM, (1.0 / frequency)) == 0 ? (1.0 / frequency) : std::fmod(TTM, (1.0 / frequency));
	forwardTimes[0] = timeToNextForwardTime;
	for (unsigned long i = 1; i < forwardTimes.size(); i++)
	{
		forwardTimes[i] = forwardTimes[i - 1] + (1.0 / frequency);
	}

	MJArray forwardFXRates(forwardTimes);
	for (unsigned long i = 0; i < forwardFXRates.size(); i++)
	{
		forwardFXRates[i] = FXRate * exp(forwardTimes[i]  * (discountingRates_domestic[i] - discountingRates_foreign[i]));
	}

	double discountedSwaps = 0;
	for (unsigned long i = 0; i < forwardTimes.size() -1; i++) //Exchange of interest rate payments
	{
		discountedSwaps += exp(-forwardTimes[i] * discountingRates_domestic[i]) * (contractRate_foreign * notional_foreign * forwardFXRates[i] - contractRate_domestic * notional_domestic) / frequency;
	}
	discountedSwaps += exp(-forwardTimes[forwardTimes.size() - 1] * discountingRates_domestic[forwardTimes.size() - 1]) * ((1 + contractRate_foreign) * notional_foreign * forwardFXRates[forwardTimes.size() - 1] - (1 + contractRate_domestic) * notional_domestic ) / frequency; //Exchange of notionals
	f = discountedSwaps;
}

void FixedForFixedFXSwapFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		discountingRates_domestic += increment;
		discountingRates_foreign += increment;
		break;
	case RiskFactor::FX_rate:
		FXRate += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void FixedForFixedFXSwapFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		discountingRates_domestic *= factor;
		discountingRates_foreign *= factor;
		break;
	case RiskFactor::FX_rate:
		FXRate *= factor;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

std::vector<std::reference_wrapper<valuationFunction>> FixedForFixedFXSwapFunction::GetInnerReference()
{
	 std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* FixedForFixedFXSwapFunction::clone() const
{
	return new FixedForFixedFXSwapFunction(*this);
}
