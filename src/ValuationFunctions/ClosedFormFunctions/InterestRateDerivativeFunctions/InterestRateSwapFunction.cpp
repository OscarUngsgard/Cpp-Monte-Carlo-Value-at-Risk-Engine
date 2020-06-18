#include "InterestRateSwapFunction.h"
#include <iostream>
#include <cmath>
#include <iostream>

InterestRateSwapFunction::InterestRateSwapFunction(std::string uniqueIdentifier_, int nominal_, double r_, double contractRate_, MJArray continousCompoundingForwardRates_, unsigned long frequency_, double TTM_) : valuationFunction(uniqueIdentifier_, TTM_, nominal_), frequency(frequency_), contractRate(contractRate_), continousCompoundingForwardRates(continousCompoundingForwardRates_)
{
	discountingRates.resize(continousCompoundingForwardRates.size());
	discountingRates = r_;
}

InterestRateSwapFunction::InterestRateSwapFunction(std::string uniqueIdentifier_, int nominal_, MJArray riskFreeRates_, double contractRate_, MJArray continousCompoundingForwardRates_, unsigned long frequency_, double TTM_) : valuationFunction(uniqueIdentifier_, TTM_, nominal_), discountingRates(riskFreeRates_), frequency(frequency_), contractRate(contractRate_), continousCompoundingForwardRates(continousCompoundingForwardRates_)
{
}

void InterestRateSwapFunction::ValueInstrument()
{
	if (discountingRates.size() != continousCompoundingForwardRates.size())
		throw("Mismatched number of forward rates and spot rates provided in IRS");
	if (floor(TTM * frequency) > discountingRates.size() && std::fmod(TTM, (1.0 / frequency)) != 0)
		throw("Mismatch between TTM, coupoun frequency or number of spot/forward rates provided.");
	MJArray forwardRates(continousCompoundingForwardRates);
	for (unsigned long i = 0; i < forwardRates.size(); i++) //continious to frequency/year compounding
	{
		forwardRates[i] = frequency * (exp(continousCompoundingForwardRates[i] / frequency) - 1);
	}

    MJArray forwardTimes(ceil(TTM * frequency));
    double timeToNextForwardTime = std::fmod(TTM, (1.0 / frequency)) == 0 ? (1.0 / frequency) : std::fmod(TTM, (1.0 / frequency));
    forwardTimes[0] = timeToNextForwardTime;
    for (unsigned long i = 1; i < forwardTimes.size(); i++)
    {
        forwardTimes[i] = forwardTimes[i - 1] + (1.0 / frequency);
    }

	if (forwardTimes.size() != forwardRates.size())
		throw("Error: Mismatched size of forward rates and swap times in Interest Rate Swap");

    double discountedSwaps = 0;
    for (unsigned long i = 0; i < forwardTimes.size(); i++)
    {
        discountedSwaps += exp(-forwardTimes[i] * discountingRates[i]) * (forwardRates[i] - contractRate)/frequency; //Positive nominal means side receiving floating and paying fixed
    }
    f = nominal * discountedSwaps;
}

void InterestRateSwapFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		continousCompoundingForwardRates += increment;
        discountingRates += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void InterestRateSwapFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		continousCompoundingForwardRates *= factor;
		discountingRates *= factor;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

std::vector<std::reference_wrapper<valuationFunction>> InterestRateSwapFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}


valuationFunction* InterestRateSwapFunction::clone() const
{
	return new InterestRateSwapFunction(*this);
}
