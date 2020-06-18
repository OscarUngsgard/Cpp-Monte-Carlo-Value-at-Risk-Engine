#include "..\include\MonteCarloVanillaOptionFunction.h"
#include "..\include\PayOff.h"
#include "..\include\StandardExcerciseOption.h"
#include "..\include\OneStepMonteCarloValuation.h"
#include "..\include\StatisticsMean.h"
#include "..\include\Random.h"

MonteCarloVanillaOptionFunction::MonteCarloVanillaOptionFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, const Wrapper<PayOff>& ThePayOff_, unsigned long numberOfPaths_) : r(r_), S(S0_), d(d_), impvol(impvol_), valuationFunction(uniqueIdentifier_, TTM_, nominal_), ThePayOff(ThePayOff_), numberOfPaths(numberOfPaths_)
{
}

void MonteCarloVanillaOptionFunction::ValueInstrument()
{
	StandardExcerciseOption thisOption(ThePayOff, TTM);
	StatisticsMean thisMeanGatherer;
	MJArray normVariates = GetArrayOfGauassiansByBoxMuller(numberOfPaths);
	OneStepMonteCarloValuation(thisOption, S, impvol, r, d, numberOfPaths, normVariates, thisMeanGatherer);
	f = nominal * thisMeanGatherer.GetResultsSoFar()[0][0];
	return;
}


void MonteCarloVanillaOptionFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		r += increment;
		break;
	case RiskFactor::equity1:
		S += increment;
		break;
	case RiskFactor::equity2:
		S += increment;
		break;
	case RiskFactor::equity3:
		S += increment;
		break;
	case RiskFactor::equity4:
		S += increment;
		break;
	case RiskFactor::equity5:
		S += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void MonteCarloVanillaOptionFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		r *= factor;
		break;
	case RiskFactor::equity1:
		S *= factor;
		break;
	case RiskFactor::equity2:
		S *= factor;
		break;
	case RiskFactor::equity3:
		S *= factor;
		break;
	case RiskFactor::equity4:
		S *= factor;
		break;
	case RiskFactor::equity5:
		S *= factor;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

std::vector<std::reference_wrapper<valuationFunction>> MonteCarloVanillaOptionFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* MonteCarloVanillaOptionFunction::clone() const
{
	return new MonteCarloVanillaOptionFunction(*this);
}