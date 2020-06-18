#pragma once
#define VALUATION_FUNCTION_H
#include "..\include\RiskFactors.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <iostream>
class valuationFunction
{
public:
	valuationFunction(std::string uniqueIdentifier_, double TTM, int nominal_);
	virtual void ValueInstrument() = 0;
	virtual double GetValue() const;
	virtual std::vector<std::string> GetuniqueIdentifier() const;
	virtual void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor) = 0;
	virtual void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor) = 0;
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference() = 0;
	virtual void UpdateTTM(double timeStep);
	const virtual int GetNominal() const;
	const virtual double GetOrigTTM() const;
	virtual valuationFunction* clone() const = 0;
	virtual ~valuationFunction() {}
private:

protected:
	std::string uniqueIdentifier;
	double f;
	double TTM;
	double origTTM;
	int nominal;
};