#pragma once
#define SIMULATION_ENGINE_H
#include <memory>
#include "..\include\valuationFunction.h"
#include "..\include\Wrapper.h"
#include "..\include\RiskFactors.h"
class SimulationEngine
{
public:
	SimulationEngine(std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor);
	virtual void DoOnePath(double horizon, double vol, double normvariate) = 0;
	virtual void UnDoOnePath(double horizon, double vol, double normvariate) = 0;
	virtual SimulationEngine* clone() const = 0;
	std::shared_ptr<valuationFunction> GetFunction();
	RiskFactor simulatedRiskFactor;
protected:
	std::shared_ptr<valuationFunction> theFunction;
};
