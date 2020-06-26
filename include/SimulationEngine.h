#pragma once
#define SIMULATION_ENGINE_H
#include <memory>
#include "valuationFunction.h"
#include "Wrapper.h"
#include "RiskFactors.h"
class SimulationEngine
{
public:
	SimulationEngine(std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor);
	virtual void ApplyReturns(double returns) = 0;
	virtual void ApplyInverseReturns(double returns) = 0;
	virtual void DoOnePath(double horizon, double vol, double normvariate) = 0;
	virtual void UnDoOnePath(double horizon, double vol, double normvariate) = 0;
	virtual SimulationEngine* clone() const = 0;
	virtual ~SimulationEngine() {}
	std::shared_ptr<valuationFunction> GetFunction();
	RiskFactor simulatedRiskFactor;
protected:
	std::shared_ptr<valuationFunction> theFunction;
};
