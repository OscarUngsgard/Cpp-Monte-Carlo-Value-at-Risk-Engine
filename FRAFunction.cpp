#include "FRAFunction.h"
#include <minmax.h>
#include <cmath>
#include <iostream>

FRAFunction::FRAFunction(std::string uniqueIdentifier_, int nominal_, double r_, double contractRate_, double forwardRate_, double TimeToForwardtime_, double TTM_) : valuationFunction(uniqueIdentifier_, TTM_), nominal(nominal_), r(r_), forwardRate(forwardRate_), contractRate(contractRate_), TimeToForwardtime(TimeToForwardtime_)
{
}

void FRAFunction::ValueInstrument()
{
	if (TimeToForwardtime > TTM)
	{
		f = 0;
	}
	else
	{
		f = nominal * exp(-r * TTM) * ( forwardRate - contractRate) * (TTM - TimeToForwardtime);
	}	
	return;
}

void FRAFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		forwardRate += increment;
		r += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void FRAFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		forwardRate *= factor;
		r *= factor;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}


std::vector<std::reference_wrapper<valuationFunction>> FRAFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

void FRAFunction::UpdateTTM(double timeStep)
{
	TTM = max(0, TTM - timeStep);
	TimeToForwardtime = max(0, TimeToForwardtime - timeStep);
	return;
}


valuationFunction* FRAFunction::clone() const
{
	return new FRAFunction(*this);
}
