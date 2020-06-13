#include "SimulationEngine.h"
SimulationEngine::SimulationEngine(double horizon_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor_) : horizon(horizon_), theFunction(theFunction_), simulatedRiskFactor(simulatedRiskFactor_)
{
}

const double SimulationEngine::GetHorizon()
{
	return horizon;
}


std::shared_ptr<valuationFunction> SimulationEngine::GetFunction()
{
	return theFunction;
}