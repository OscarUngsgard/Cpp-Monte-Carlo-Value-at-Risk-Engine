#include "OutPerformanceFunction.h"
#include "Normals.h"
#include <math.h>
OutPerformanceFunction::OutPerformanceFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S_vect_, std::vector<double> startingValues_vect, double r_, std::vector<double> d_vect_, std::vector<double> impvol_vect_, double corr_, double TTM_)
	: r(r_), S_vect(S_vect_), d_vect(d_vect_), impvol_vect(impvol_vect_), corr(corr_), S_start_vect(startingValues_vect), valuationFunction(uniqueIdentifier_, TTM_, nominal_)
{
}

OutPerformanceFunction::OutPerformanceFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S_vect_, std::vector<double> startingValues_vect_, double r_, std::vector<double> d_vect_, std::vector<double> impvol_vect_, std::vector<std::vector<double>> covMat, double TTM_)
	: r(r_), S_vect(S_vect_), d_vect(d_vect_), impvol_vect(impvol_vect_), S_start_vect(startingValues_vect_), valuationFunction(uniqueIdentifier_, TTM_, nominal_)
{
	corr = covMat[0][1] / sqrt(covMat[0][0] * covMat[1][1]);
}

void OutPerformanceFunction::ValueInstrument()
{
	/// Using the Margrabe formula for the relative price path
	double sigma = sqrt(pow(impvol_vect[0],2) + pow(impvol_vect[1],2) - 2.0 * impvol_vect[0] * impvol_vect[1] * corr);
	double d1 = log(((S_vect[0] / S_start_vect[0]) / (S_vect[1] / S_start_vect[1])) + (d_vect[1] - d_vect[0] + (pow(sigma, 2) / 2.0) * TTM)) / (sigma * sqrt(TTM));
	double d2 = d1 - sigma * sqrt(TTM);
	f = nominal * (exp(-d_vect[0] * TTM) * (S_vect[0] / S_start_vect[0]) * CumulativeNormal(d1) - exp(-d_vect[1] * TTM) * (S_vect[1] / S_start_vect[1]) * CumulativeNormal(d2));
	return;
}

void OutPerformanceFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
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

void OutPerformanceFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
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
std::vector<std::reference_wrapper<valuationFunction>> OutPerformanceFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* OutPerformanceFunction::clone() const
{
    return new OutPerformanceFunction(*this);
}
