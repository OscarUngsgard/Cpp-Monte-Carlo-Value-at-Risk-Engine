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
	OneStepBSEngine(double horizon_, double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor);
	void DoOnePath(double vol, double normvariate);
	void UnDoOnePath(double vol, double normvariate);
	virtual SimulationEngine* clone() const;
private:
	double drift;
};
