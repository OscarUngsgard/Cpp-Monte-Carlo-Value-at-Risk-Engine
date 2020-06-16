#include "AmericanPutFunction.h"
#include "TreeAmerican.h"
#include "BinomialTree.h"
#include "PayOffPut.h"

AmericanPutFunction::AmericanPutFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, double strike_, unsigned long binomTreeSteps_) : r(r_), S(S0_), d(d_), impvol(impvol_), valuationFunction(uniqueIdentifier_, TTM_, nominal_), strike(strike_), binomTreeSteps(binomTreeSteps_)
{
}


void AmericanPutFunction::ValueInstrument()
{
	if (TTM == 0)
	{
		PayOffPut thePayOff(strike);
		f = nominal * thePayOff(S);
		return;
	}
	SimpleBinomialTree theTree(S, r, d, impvol, binomTreeSteps, TTM);
	PayOffPut thePayOff(strike);
	TreeAmerican americanOption(TTM, thePayOff);
	double price1 = theTree.GetThePrice(americanOption);
	binomTreeSteps++; //Calculate again with another step and take average of results
	SimpleBinomialTree theNewTree(S, r, d, impvol, binomTreeSteps, TTM);
	double price2 = theNewTree.GetThePrice(americanOption);
	f = nominal * ((price1 + price2) / 2);
	binomTreeSteps--; //Make sure to reset number of steps to original for next calculation
	return;
}


void AmericanPutFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
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

void AmericanPutFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
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

std::vector<std::reference_wrapper<valuationFunction>> AmericanPutFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}



valuationFunction* AmericanPutFunction::clone() const
{
	return new AmericanPutFunction(*this);
}
