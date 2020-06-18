#include "MonteCarloRainbowOptionFunction.h"
#include "PayOff.h"
#include "StandardExcerciseOption.h"
#include "OneStepMonteCarloValuation.h"
#include "StatisticAllPaths.h"
#include "Random.h"
#include <algorithm>
#include <iostream>

MonteCarloRainbowOptionFunction::MonteCarloRainbowOptionFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S0_vect, double r_, std::vector<double> d_vect_, std::vector<double> impvol_vect_, std::vector<std::vector<double>> covMatrix_, double TTM_, std::vector<Wrapper<PayOff>> ThePayOffVect_, unsigned long numberOfPaths_, RainbowOptionType optionType_) : r(r_), S_vect(S0_vect), d_vect(d_vect_), covMatrix(covMatrix_), valuationFunction(uniqueIdentifier_, TTM_, nominal_),ThePayOffVect(std::move(ThePayOffVect_)), numberOfPaths(numberOfPaths_), optionType(optionType_), impvol_vect(impvol_vect_)
{
	if (covMatrix.size() != S_vect.size())
		throw("Missmatched Covariance matrix and initial spot values array sizes in MonteCarloRainbowOptionFunction");
}

void MonteCarloRainbowOptionFunction::ValueInstrument()
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
		switch (optionType)
		{
		case RainbowOptionType::best_of:
			f += *max_element(outcomes.begin(), outcomes.end());
			break;
		case RainbowOptionType::worst_of:
			f += *min_element(outcomes.begin(), outcomes.end());
			break;			
		default:
			throw std::runtime_error("Unsupported or unknown option type found in " + GetuniqueIdentifier()[0]);
			break;
		}	
	}
	f *= ((double)nominal/numberOfPaths);
	return;
}

void MonteCarloRainbowOptionFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
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
	case RiskFactor::equity3:
		S_vect[2] += increment;
		break;
	case RiskFactor::equity4:
		S_vect[3] += increment;
		break;
	case RiskFactor::equity5:
		S_vect[4] += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void MonteCarloRainbowOptionFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
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
	case RiskFactor::equity3:
		S_vect[2] *= factor;
		break;
	case RiskFactor::equity4:
		S_vect[3] *= factor;
	case RiskFactor::equity5:
		S_vect[4] *= factor;
		break;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

std::vector<std::reference_wrapper<valuationFunction>> MonteCarloRainbowOptionFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* MonteCarloRainbowOptionFunction::clone() const
{
	return new MonteCarloRainbowOptionFunction(*this);
}