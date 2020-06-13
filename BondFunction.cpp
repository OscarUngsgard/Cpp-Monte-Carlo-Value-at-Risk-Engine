#include "BondFunction.h"
#include "Arrays.h"
#include <cmath>
#include <iostream>
BondFunction::BondFunction(std::string uniqueIdentifier_, int nominal_, double yield_, double faceValue_, double couponRate_, int couponFreq_, double TTM_) : valuationFunction(uniqueIdentifier_, TTM_), yield(yield_), nominal(nominal_), faceValue(faceValue_), couponRate(couponRate_), couponFreq(couponFreq_)
{
}

void BondFunction::ValueInstrument()
{
    MJArray couponTimes(ceil(TTM * couponFreq));
    double timeToNextCoupon = std::fmod(TTM, (1.0 / couponFreq)) == 0 ? (1.0 / couponFreq) : std::fmod(TTM, (1.0 / couponFreq));
    couponTimes[0] = timeToNextCoupon;
    for (unsigned long i = 1; i < couponTimes.size(); i++)
    {
        couponTimes[i] = couponTimes[i - 1] + (1.0 / couponFreq);
    }

    double coupon = couponRate / couponFreq;
    double accruedInterest = timeToNextCoupon * couponRate * faceValue;
    double dirtyprice = 0;
    for (unsigned long i = 0; i < couponTimes.size()-1; i++)
    {
        dirtyprice += exp(-yield * couponTimes[i]) * coupon * faceValue;
    }
    dirtyprice += exp(-yield * couponTimes[couponTimes.size() - 1]) * faceValue * (1 + coupon);
    double cleanprice = dirtyprice - accruedInterest;
    f = nominal * cleanprice;
}


void BondFunction::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		yield += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void BondFunction::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		yield *= factor;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

std::vector<std::reference_wrapper<valuationFunction>> BondFunction::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}


valuationFunction* BondFunction::clone() const
{
	return new BondFunction(*this);
}
