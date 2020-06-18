#pragma once
#define BSENGINE_H
#include "..\include\MCStatistics.h"
#include "..\include\Random.h"
#include "..\include\MCEngine.h"
#include "..\include\valuationFunction.h"
#include "..\include\SimulationEngine.h"
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
