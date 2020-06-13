#include "FXForwardFunction.h"
#include <cmath>

FXForwardFunction::FXForwardFunction(std::string uniqueIdentifier_, int nominal_, double X_, double r_domestic_, double r_foreign_, double TTM_, double strike_) : r_domestic(r_domestic_), X(X_), r_foreign(r_foreign_), valuationFunction(uniqueIdentifier_, TTM_), nominal(nominal_), strike(strike_)
{
}

void FXForwardFunction::ValueInstrument()
{
	f = nominal * (X * exp(-r_foreign * TTM) - strike * exp(-r_domestic * TTM));
	return;
}

void FXForwardFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		r_foreign += increment;
		r_domestic += increment;
		break;
	case RiskFactor::FX_rate:
		X += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
}

void FXForwardFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		r_foreign *= factor;
		r_domestic *= factor;
		break;
	case RiskFactor::FX_rate:
		X *= factor;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
}

std::vector<std::reference_wrapper<valuationFunction>> FXForwardFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}


valuationFunction* FXForwardFunction::clone() const
{
	return nullptr;
}
