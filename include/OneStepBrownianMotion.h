#pragma once
#define ONE_STEP_BROWNIAN_MOTION_ENGINE_H
#include "..\include\MCStatistics.h"
#include "..\include\Random.h"
#include "..\include\MCEngine.h"
#include "..\include\valuationFunction.h"
#include "..\include\SimulationEngine.h"
class OneStepBrownianMotionEngine : public SimulationEngine
{
public:
	OneStepBrownianMotionEngine(double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor_);
	virtual SimulationEngine* clone() const;
	void DoOnePath(double horizon, double vol, double normvariate);
	void UnDoOnePath(double horizon, double vol, double normvariate);
private:
	double drift;
};
