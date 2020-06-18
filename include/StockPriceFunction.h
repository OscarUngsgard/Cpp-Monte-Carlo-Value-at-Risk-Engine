#pragma once
#define STOCK_PRICE_FUNCTION_H
#include "..\include\valuationFunction.h"
class StockPriceFunction : public valuationFunction
{
public:
	StockPriceFunction(std::string uniqueIdentifier_, int nominal_, double S);
	void ValueInstrument();
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	double S;
};