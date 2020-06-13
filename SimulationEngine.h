#pragma once
#define SIMULATION_ENGINE_H
#include <memory>
#include "valuationFunction.h"
#include "Wrapper.h"
#include "RiskFactors.h"
class SimulationEngine
{
public:
	SimulationEngine(double horizon, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor);
	virtual void DoOnePath(double vol, double normvariate) = 0;
	virtual void UnDoOnePath(double vol, double normvariate) = 0;
	virtual SimulationEngine* clone() const = 0;
	const double GetHorizon();
	std::shared_ptr<valuationFunction> GetFunction();
	RiskFactor simulatedRiskFactor;
protected:
	double horizon;
	std::shared_ptr<valuationFunction> theFunction;
};
