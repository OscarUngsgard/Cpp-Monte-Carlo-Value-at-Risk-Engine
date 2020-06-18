#include "..\include\PayOffStock.h"
#include "..\include\StandardExcerciseOption.h"
#include "..\include\OneStepMonteCarloValuation.h"
#include "..\include\StatisticAllPaths.h"
#include "..\include\Random.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include "..\include\MonteCarloBasketOptionFunction.h"

MonteCarloBasketOptionFunction::MonteCarloBasketOptionFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S0_vect, std::vector<double> weights_vect_, double r_, std::vector<double> d_vect_, std::vector<double> impvol_vect_, std::vector<std::vector<double>> covMatrix_, double TTM_, Wrapper<PayOff> ThePayOff_, unsigned long numberOfPaths_) : r(r_), S_vect(S0_vect), weights_vect(weights_vect_), d_vect(d_vect_), covMatrix(covMatrix_), valuationFunction(uniqueIdentifier_, TTM_, nominal_), ThePayOff(std::move(ThePayOff_)), numberOfPaths(numberOfPaths_), impvol_vect(impvol_vect_)
{
	if (covMatrix.size() != S_vect.size())
		throw("Missmatched Covariance matrix and initial spot values array sizes in MonteCarloBasketOptionFunction");
}


void MonteCarloBasketOptionFunction::ValueInstrument()
{
	std::vector<MJArray> correlatedNormVariates = GetArraysOfCorrelatedGauassiansByBoxMuller(numberOfPaths, covMatrix);
	std::vector<StatisticAllPaths> thesePathGatherers;
	PayOffStock aStockPayOff;
	std::vector<PayOffStock> stockPayOffVect(S_vect.size(), aStockPayOff);
	for (unsigned long i = 0; i < S_vect.size(); i++)
	{
		StandardExcerciseOption thisStockPayOff(stockPayOffVect[i], TTM);
		StatisticAllPaths onePathGatherer;
		thesePathGatherers.push_back(onePathGatherer);
		OneStepMonteCarloValuation(thisStockPayOff, S_vect[i], impvol_vect[i], r, d_vect[i], numberOfPaths, correlatedNormVariates[i], thesePathGatherers[i]);
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
		double basketValue = 0;
		for (unsigned long i = 0; i < outcomes.size(); i++)
		{
			basketValue += outcomes[i] * weights_vect[i]; //Final price of the equity basket
		}
		f += ThePayOff->operator()(basketValue);
	}
	f *= ((double)nominal / numberOfPaths);
	return;
}

void MonteCarloBasketOptionFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
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

void MonteCarloBasketOptionFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
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

std::vector<std::reference_wrapper<valuationFunction>> MonteCarloBasketOptionFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* MonteCarloBasketOptionFunction::clone() const
{
	return new MonteCarloBasketOptionFunction(*this);
}


