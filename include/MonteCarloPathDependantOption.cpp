#include "MonteCarloPathDependantOption.h"
#include "PathDependantPayOff.h"
#include "PathDependantOption.h"
#include "DiscretizedPathMonteCarloValuation.h"
#include "Random.h"
#include "StatisticsMean.h"

MonteCarloPathDependantOptionFunction::MonteCarloPathDependantOptionFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, const Wrapper<PathDependantPayOff>& ThePayOff_, unsigned long numberOfPaths_, unsigned long numberOfSteps_) : r(r_), S(S0_), d(d_), impvol(impvol_), valuationFunction(uniqueIdentifier_, TTM_, nominal_), ThePayOff(ThePayOff_), numberOfPaths(numberOfPaths_), numberOfSteps(numberOfSteps_)
{
}


void MonteCarloPathDependantOptionFunction::ValueInstrument()
{
	PathDependantOption thisOption(ThePayOff, TTM);
	StatisticsMean thisMeanGatherer;
	MJArray normVariates = GetArrayOfGauassiansByBoxMuller(numberOfPaths * numberOfSteps);
	DiscretizedPathMonteCarloValuation(thisOption, S, impvol, r, d, numberOfPaths, numberOfSteps, normVariates, thisMeanGatherer);
	f = nominal * thisMeanGatherer.GetResultsSoFar()[0][0];
	return;
}


void MonteCarloPathDependantOptionFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
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

void MonteCarloPathDependantOptionFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
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

std::vector<std::reference_wrapper<valuationFunction>> MonteCarloPathDependantOptionFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* MonteCarloPathDependantOptionFunction::clone() const
{
	return new MonteCarloPathDependantOptionFunction(*this);
}