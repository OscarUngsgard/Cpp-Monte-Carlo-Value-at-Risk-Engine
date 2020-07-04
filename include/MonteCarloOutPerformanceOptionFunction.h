#pragma once
#define MONTE_CARLO_OUTPERFORMANCE_OPTION_FUNCTION_H
#include "valuationFunction.h"
#include "PayOff.h"
#include "RainbowOptionTypes.h"
#include "Wrapper.h"
class MonteCarloOutPerformanceOptionFunction : public valuationFunction
{
public:
	MonteCarloOutPerformanceOptionFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S0_vect, std::vector<Wrapper<PayOff>> ThePayOffVect_, double r_, std::vector<double> d_vect, std::vector<double> impvol_vect_, std::vector<std::vector<double>> covMatrix_, double TTM_, unsigned long numberOfPaths_);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	unsigned long numberOfPaths;
	double r;
	std::vector<double> S_vect;
	std::vector<double> S0_start_vect;
	std::vector<double> d_vect;
	std::vector<double> impvol_vect;
	std::vector<std::vector<double>> covMatrix;
	std::vector<Wrapper<PayOff>> ThePayOffVect;
};