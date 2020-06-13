#include "OneStepMonteCarloValuation.h"
#include "Random.h"


void OneStepMonteCarloValuation(const StandardExcerciseOption& TheOption, double Spot, double Vol, double r, double d, unsigned long NumberOfPaths, MJArray normVariates, StatisticsMC& gatherer)
{
    if (normVariates.size() != NumberOfPaths)
        throw("mismatched number of paths and normal variates");
    //Pre-calculate as much as possible
    double Expiry = TheOption.GetExpiry();
    double variance = Vol * Vol * Expiry;
    double rootVariance = sqrt(variance);
    double itoCorrection = -0.5 * variance;
    double movedSpot = Spot * exp((r-d) * Expiry + itoCorrection);
    double thisSpot;
    double discounting = exp(-r * Expiry);
    for (unsigned long i = 0; i < NumberOfPaths; i++)
    {
        thisSpot = movedSpot * exp(rootVariance * normVariates[i]);
        double thisPayoff = TheOption.OptionPayOff(thisSpot);
        gatherer.DumpOneResult(discounting * thisPayoff);
    }
    return;
}
