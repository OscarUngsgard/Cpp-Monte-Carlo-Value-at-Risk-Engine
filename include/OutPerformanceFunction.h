#pragma once
#define OUTPERFORMANCE_FUNCTION_H
#include "valuationFunction.h"
//Outperformance of first stock over second
class OutPerformanceFunction : public valuationFunction
{
public:
	OutPerformanceFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S_vect, std::vector<double> startingValues_vect, double r_, std::vector<double> d_vect, std::vector<double> impvol_vect, double corr_, double TTM_);
	OutPerformanceFunction(std::string uniqueIdentifier_, int nominal_, std::vector<double> S_vect, std::vector<double> startingValues_vect, double r_, std::vector<double> d_vect, std::vector<double> impvol_vect, std::vector<std::vector<double>> covMat, double TTM_);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	std::vector<double> S_vect;
	std::vector<double> S_start_vect;
	double r;
	std::vector<double> d_vect;
	std::vector<double> impvol_vect;
	double corr;
};