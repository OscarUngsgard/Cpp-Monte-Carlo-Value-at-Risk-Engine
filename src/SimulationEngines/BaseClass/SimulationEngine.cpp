#include "SimulationEngine.h"
SimulationEngine::SimulationEngine(std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor_) : theFunction(theFunction_), simulatedRiskFactor(simulatedRiskFactor_)
{
}

std::shared_ptr<valuationFunction> SimulationEngine::GetFunction()
{
	return theFunction;
}