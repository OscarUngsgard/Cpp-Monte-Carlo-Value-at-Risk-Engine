#pragma once
#define BSENGINE_H
#include "MCStatistics.h"
#include "Random.h"
#include "MCEngine.h"
#include "valuationFunction.h"
#include "SimulationEngine.h"
class OneStepBSEngine : public SimulationEngine
{
public:
	OneStepBSEngine(double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor);
	void DoOnePath(double horizon, double vol, double normvariate);
	void UnDoOnePath(double horizon, double vol, double normvariate);
	virtual SimulationEngine* clone() const;
private:
	double drift;
};
