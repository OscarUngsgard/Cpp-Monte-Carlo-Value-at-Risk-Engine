#include "AmericanCallFunction.h"
#include "TreeAmerican.h"
#include "BinomialTree.h"
#include <iostream>
//Makes use of the binomial tree code provided in C++ Design Patterns, augmented to use risk neutral probabilities of up and down movements to make the implied discounted price a martingale
AmericanCallFunction::AmericanCallFunction(std::string uniqueIdentifier_, int nominal_, double S_, double r_, double d_, double impvol_, double TTM_, double strike_, unsigned long binomTreeSteps_) : r(r_), S(S_), d(d_), impvol(impvol_), valuationFunction(uniqueIdentifier_, TTM_), nominal(nominal_), strike(strike_), binomTreeSteps(binomTreeSteps_)
{
}

void AmericanCallFunction::ValueInstrument()
{
	SimpleBinomialTree theTree(S, r, d, impvol, binomTreeSteps, TTM);
	PayOffCall thePayOff(strike);
	TreeAmerican americanOption(TTM, thePayOff);
	double price1 = theTree.GetThePrice(americanOption);
	binomTreeSteps++; //Calculate again with another step and take average of results
	SimpleBinomialTree theNewTree(S, r, d, impvol, binomTreeSteps, TTM);
	double price2 = theNewTree.GetThePrice(americanOption);
	f = nominal * ((price1 + price2) / 2);
	binomTreeSteps--; //Make sure to reset number of steps to original for next calculation
	return;
}

void AmericanCallFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
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
		throw("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void AmericanCallFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
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

std::vector<std::reference_wrapper<valuationFunction>> AmericanCallFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.reserve(1);
	innerVector.push_back(std::ref(*this));
	return innerVector;
}


valuationFunction* AmericanCallFunction::clone() const
{
	return new AmericanCallFunction(*this);
}
