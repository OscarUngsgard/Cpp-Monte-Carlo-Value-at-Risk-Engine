#include "BSCallFunction.h"
#include "Random.h"
#include <cmath>
#include <minmax.h>
#include "Normals.h"
#include <iostream>
BSCallFunction::BSCallFunction(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, double strike_) : r(r_), S(S0_), d(d_), impvol(impvol_), valuationFunction(uniqueIdentifier_, TTM_), nominal(nominal_), strike(strike_) //nominal(nominal_),
{
}

void BSCallFunction::ValueInstrument()
{
	double d1 = (log(S / strike) + (r - d + 0.5 * impvol* impvol) * TTM) / (impvol * sqrt(TTM));
	double d2 = d1 - impvol * sqrt(TTM);
	f = nominal * (S * exp(-d * TTM)  * CumulativeNormal(d1) - strike * exp(-r * TTM) * CumulativeNormal(d2));
	return;
}

void BSCallFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
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

void BSCallFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
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

std::vector<std::reference_wrapper<valuationFunction>> BSCallFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* BSCallFunction::clone() const
{
	return new BSCallFunction(*this);
}
