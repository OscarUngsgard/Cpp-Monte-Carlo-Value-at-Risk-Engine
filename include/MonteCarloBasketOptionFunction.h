#pragma once
#define MONTE_CARLO_BASKET_OPTION_FUNCTION_H
#include "valuationFunction.h"
#include "PayOff.h"
#include "RainbowOptionTypes.h"
#include "Wrapper.h"
class MonteCarloBasketOptionFunction : public valuationFunction
{
public:
	MonteCarloBasketOptionFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S0_vect, std::vector<double> weights_vect_, std::vector<double>  r_Vect_, std::vector<double> d_vect, std::vector<double> impvol_vect_, std::vector<std::vector<double>> covMatrix_, double TTM_, Wrapper<PayOff> ThePayOff_, unsigned long numberOfPaths_);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	unsigned long numberOfPaths;
	std::vector<double>  r_Vect;
	std::vector<double> S_vect;
	std::vector<double> weights_vect;
	std::vector<double> d_vect;
	Wrapper<PayOff> ThePayOff;
	std::vector<double> impvol_vect;
	std::vector<std::vector<double>> covMatrix;
};
