#include "OneStepBrownianMotion.h"
#include <iostream>

OneStepBrownianMotionEngine::OneStepBrownianMotionEngine(double horizon_, double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor_) : SimulationEngine(horizon_, theFunction_, simulatedRiskFactor_), drift(drift_)
{
}


void OneStepBrownianMotionEngine::DoOnePath(double vol, double normvariate)
{
    double variance = vol * vol * horizon;
    double rootVariance = sqrt(variance);
    double increment = (horizon * drift + rootVariance * normvariate);
    theFunction->RiskFactorAdd(increment, simulatedRiskFactor);
    return;
}

void OneStepBrownianMotionEngine::UnDoOnePath(double vol, double normvariate)
{
    double variance = vol * vol * horizon;
    double rootVariance = sqrt(variance);
    double increment = (horizon * drift + rootVariance * normvariate);
    theFunction->RiskFactorAdd(-increment, simulatedRiskFactor);
    return;
}

SimulationEngine* OneStepBrownianMotionEngine::clone() const
{
    return new OneStepBrownianMotionEngine(*this);
}

