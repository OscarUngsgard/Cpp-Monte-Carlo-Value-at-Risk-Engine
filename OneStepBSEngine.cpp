#include "OneStepBSEngine.h"
#include <iostream>

OneStepBSEngine::OneStepBSEngine(double horizon_, double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor_) : SimulationEngine(horizon_, theFunction_, simulatedRiskFactor_), drift(drift_)
{
}

void OneStepBSEngine::DoOnePath(double vol, double normvariate)
{
    double variance = vol * vol * horizon;
    double rootVariance = sqrt(variance);
    double itoCorrection = -0.5 * variance;
    double factor = exp(drift * horizon + itoCorrection + rootVariance * normvariate);
    theFunction->RiskFactorMultiply(factor, simulatedRiskFactor);
    return;
}

void OneStepBSEngine::UnDoOnePath(double vol, double normvariate)
{
    double variance = vol * vol * horizon;
    double rootVariance = sqrt(variance);
    double itoCorrection = -0.5 * variance;
    double factor = exp(drift * horizon + itoCorrection + rootVariance * normvariate);
    theFunction->RiskFactorMultiply(1/factor, simulatedRiskFactor);
    return;
}

SimulationEngine* OneStepBSEngine::clone() const
{
    return new OneStepBSEngine(*this);
}