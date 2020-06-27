#include "OneStepBSEngine.h"
#include <iostream>
#include <algorithm>

OneStepBSEngine::OneStepBSEngine(double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor_) : SimulationEngine(theFunction_, simulatedRiskFactor_), drift(drift_)
{
}

void OneStepBSEngine::DoOnePath(double horizon, double vol, double normvariate)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        double adjustedHorizon = std::min(horizon, innerReferences[i].get().GetOrigTTM());  //If original TTM is lower than the VaR time horizion then only simulate up to that point
        double variance = vol * vol * adjustedHorizon;
        double rootVariance = sqrt(variance);
        double itoCorrection = -0.5 * variance;
        double factor = exp(drift * adjustedHorizon + itoCorrection + rootVariance * normvariate);
        innerReferences[i].get().RiskFactorMultiply(factor, simulatedRiskFactor);
    }
    return;
}

void OneStepBSEngine::UnDoOnePath(double horizon, double vol, double normvariate)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        double adjustedHorizon = std::min(horizon, innerReferences[i].get().GetOrigTTM());  //If original TTM is lower than the VaR time horizion then only simulate up to that point
        double variance = vol * vol * adjustedHorizon;
        double rootVariance = sqrt(variance);
        double itoCorrection = -0.5 * variance;
        double factor = exp(drift * adjustedHorizon + itoCorrection + rootVariance * normvariate);
        innerReferences[i].get().RiskFactorMultiply(1/factor, simulatedRiskFactor);
    }
}

void OneStepBSEngine::ApplyReturns(double returns)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        innerReferences[i].get().RiskFactorMultiply(exp(returns), simulatedRiskFactor);
    }
    return;
}

void OneStepBSEngine::ApplyInverseReturns(double returns)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        innerReferences[i].get().RiskFactorMultiply(1 / exp(returns), simulatedRiskFactor);
    }
    return;
}


SimulationEngine* OneStepBSEngine::clone() const
{
    return new OneStepBSEngine(*this);
}