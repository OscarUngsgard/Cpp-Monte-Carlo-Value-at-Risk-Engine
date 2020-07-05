#include "MonteCarloOutPerformanceOptionFunction.h"
#include "PayOff.h"
#include "PayOffStock.h"
#include "StandardExcerciseOption.h"
#include "OneStepMonteCarloValuation.h"
#include "StatisticAllPaths.h"
#include "Random.h"
#include <algorithm>
#include <iostream>
#include <algorithm>

MonteCarloOutPerformanceOptionFunction::MonteCarloOutPerformanceOptionFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S0_vect, std::vector<Wrapper<PayOff>> ThePayOffVect_, double r_, std::vector<double> d_vect_, std::vector<double> impvol_vect_, std::vector<std::vector<double>> covMatrix_, double TTM_, unsigned long numberOfPaths_)
	: r(r_), S_vect(S0_vect), ThePayOffVect(ThePayOffVect_), d_vect(d_vect_), covMatrix(covMatrix_), valuationFunction(uniqueIdentifier_, TTM_, nominal_), numberOfPaths(numberOfPaths_), impvol_vect(impvol_vect_)
{
	if (covMatrix.size() != S_vect.size())
		throw("Missmatched Covariance matrix and initial spot values array sizes in OutPerformance Option");
	if (2 != S_vect.size())
		throw("More than two equities specified in OutPerformance Option");
}


void MonteCarloOutPerformanceOptionFunction::ValueInstrument()
{
	std::vector<MJArray> correlatedNormVariates = GetArraysOfCorrelatedGauassiansByBoxMuller(numberOfPaths, covMatrix);
	std::vector<StatisticAllPaths> thesePathGatherers;
	for (unsigned long i = 0; i < S_vect.size(); i++)
	{
		StandardExcerciseOption thisOption(ThePayOffVect[i], TTM);
		StatisticAllPaths onePathGatherer;
		thesePathGatherers.push_back(onePathGatherer);
		OneStepMonteCarloValuation(thisOption, S_vect[i], impvol_vect[i], r, d_vect[i], numberOfPaths, correlatedNormVariates[i], thesePathGatherers[i]);
	}
	f = 0;
	for (unsigned long i = 0; i < numberOfPaths; i++)
	{
		std::vector<double> outcomes;
		outcomes.reserve(S_vect.size());
		for (unsigned long j = 0; j < S_vect.size(); j++)
		{
			outcomes.push_back(thesePathGatherers[j].GetOneValueFromResultsSoFar(i));
		}
		f += std::max(outcomes[0] - outcomes[1], 0.0);
	}
	f *= ((double)nominal / numberOfPaths);
	return;
}

void MonteCarloOutPerformanceOptionFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		r += increment;
		break;
	case RiskFactor::equity1:
		S_vect[0] += increment;
		break;
	case RiskFactor::equity2:
		S_vect[1] += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void MonteCarloOutPerformanceOptionFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		r *= factor;
		break;
	case RiskFactor::equity1:
		S_vect[0] *= factor;
		break;
	case RiskFactor::equity2:
		S_vect[1] *= factor;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

std::vector<std::reference_wrapper<valuationFunction>> MonteCarloOutPerformanceOptionFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* MonteCarloOutPerformanceOptionFunction::clone() const
{
	return new MonteCarloOutPerformanceOptionFunction(*this);
}