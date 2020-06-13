#include "FunctionCombiner.h"
#include <iostream>
FunctionCombiner::FunctionCombiner(std::vector<std::shared_ptr<valuationFunction>> Inner_) : Inner(std::move(Inner_)), valuationFunction("", 0) //<- Needs to initalize itself even though it gets all data from inner functions.
{
}

void FunctionCombiner::ValueInstrument()
{
	f = 0;
	for (unsigned long i = 0; i < Inner.size(); ++i) {
		Inner[i]->ValueInstrument();
		f += Inner[i]->GetValue();
	}
	return;
}

std::vector<std::string> FunctionCombiner::GetuniqueIdentifier() const
{
	std::vector<std::string> stringVector;
	for (unsigned long i = 0; i < Inner.size(); ++i) 
	{
		std::vector<std::string> innerStringVector = Inner[i]->GetuniqueIdentifier();
		stringVector.insert(stringVector.end(), innerStringVector.begin(), innerStringVector.end());
	}
	return stringVector;
}

void FunctionCombiner::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	for (unsigned long i = 0; i < Inner.size(); ++i) {
		Inner[i]->RiskFactorAdd(increment, simulatedRiskFactor);
	}
	return;
}

void FunctionCombiner::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	for (unsigned long i = 0; i < Inner.size(); ++i) {
		Inner[i]->RiskFactorMultiply(factor, simulatedRiskFactor);
	}
	return;
}

std::vector<std::reference_wrapper<valuationFunction>> FunctionCombiner::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	std::vector<std::reference_wrapper<valuationFunction>> innerInnerVector;
	innerVector.reserve(Inner.size());
	for (unsigned long i = 0; i < Inner.size(); ++i) {
		innerInnerVector = Inner[i]->GetInnerReference();
		innerVector.insert(innerVector.end(), innerInnerVector.begin(), innerInnerVector.end());
	}
	return innerVector;
}


valuationFunction* FunctionCombiner::clone() const
{
	return new FunctionCombiner(*this);	
}
