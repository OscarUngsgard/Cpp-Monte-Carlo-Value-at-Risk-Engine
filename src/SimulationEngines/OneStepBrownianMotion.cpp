#include "OneStepBrownianMotion.h"
#include <iostream>
#include <algorithm>

OneStepBrownianMotionEngine::OneStepBrownianMotionEngine(double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor_) : SimulationEngine(theFunction_, simulatedRiskFactor_), drift(drift_)
{
}


void OneStepBrownianMotionEngine::DoOnePath(double horizon, double vol, double normvariate)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        double adjustedHorizon = std::min(horizon, innerReferences[i].get().GetOrigTTM());
        double variance = vol * vol * horizon;
        double rootVariance = sqrt(variance);
        double increment = (horizon * drift + rootVariance * normvariate);
        innerReferences[i].get().RiskFactorAdd(increment, simulatedRiskFactor);
    }
    return;
}

void OneStepBrownianMotionEngine::UnDoOnePath(double horizon, double vol, double normvariate)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        double adjustedHorizon = std::min(horizon, innerReferences[i].get().GetOrigTTM());
        double variance = vol * vol * horizon;
        double rootVariance = sqrt(variance);
        double increment = (horizon * drift + rootVariance * normvariate);
        innerReferences[i].get().RiskFactorAdd(-increment, simulatedRiskFactor);
    }
    return;
}

SimulationEngine* OneStepBrownianMotionEngine::clone() const
{
    return new OneStepBrownianMotionEngine(*this);
}

