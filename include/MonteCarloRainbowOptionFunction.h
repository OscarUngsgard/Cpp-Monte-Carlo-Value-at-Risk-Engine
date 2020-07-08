#pragma once
#define MONTE_CARLO_RAINBOW_OPTION_FUNCTION_H
#include "valuationFunction.h"
#include "PayOff.h"
#include "RainbowOptionTypes.h"
#include "Wrapper.h"
class MonteCarloRainbowOptionFunction : public valuationFunction
{
public:
	MonteCarloRainbowOptionFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S0_vect, std::vector<double> r_Vect_, std::vector<double> d_vect, std::vector<double> impvol_vect_, std::vector<std::vector<double>> covMatrix_, double TTM_, std::vector<Wrapper<PayOff>> ThePayOffVect_, unsigned long numberOfPaths_, RainbowOptionType optionType_);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	unsigned long numberOfPaths;
	std::vector<double> r_Vect;
	std::vector<double> S_vect;
	std::vector<double> d_vect;
	std::vector<Wrapper<PayOff>> ThePayOffVect;	
	std::vector<double> impvol_vect;
	std::vector<std::vector<double>> covMatrix;
	RainbowOptionType optionType;
};