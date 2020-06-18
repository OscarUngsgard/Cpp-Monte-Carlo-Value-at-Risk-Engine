#pragma once
#define FUNCTION_COMBINER_H
#include "..\include\valuationFunction.h"
#include <memory>
#include "..\include\Wrapper.h"
#include <vector>
class FunctionCombiner : public valuationFunction
{
public:
	FunctionCombiner(std::vector<std::shared_ptr<valuationFunction>> Inner_);
	void ValueInstrument();
	std::vector<std::string> GetuniqueIdentifier() const;
	void RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor);
	void RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor);
	virtual std::vector<std::reference_wrapper<valuationFunction>> GetInnerReference();
	virtual valuationFunction* clone() const;
private:
	std::vector<std::shared_ptr<valuationFunction>> Inner;
};