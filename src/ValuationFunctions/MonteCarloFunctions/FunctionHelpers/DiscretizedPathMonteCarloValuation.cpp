#include "DiscretizedPathMonteCarloValuation.h"
#include "Random.h"
#include "PathDependantOption.h"
#include <iostream>

void DiscretizedPathMonteCarloValuation(const PathDependantOption& TheOption, double spot, double vol, double r, double d, unsigned long numberOfPaths, unsigned long numberOfSteps, MJArray normVariates, StatisticsMC& gatherer)
{
    if (normVariates.size() != (numberOfPaths * numberOfSteps))
        throw("mismatched number of paths and normal variates");
    //Pre-calculate as much as possible
    double Expiry = TheOption.GetExpiry();
    double discounting = exp(-r * Expiry);   
    double dt = Expiry / numberOfPaths;
    double logDrift = (r - d - (vol * vol) / 2.0) * dt;
    double logDifussion = vol * sqrt(dt);
    double CurrentLogSpot = log(spot);
    for (unsigned long i = 0; i < numberOfPaths; i++)
    {
        std::vector<double> SpotValues(numberOfSteps);
        for (unsigned long j = 0; j < numberOfSteps; j++)
        {       
        CurrentLogSpot += logDrift;
        CurrentLogSpot += logDifussion * normVariates[numberOfSteps * i + j];
        SpotValues[j] = exp(CurrentLogSpot);
        }
        double thisPayoff = TheOption.OptionPayOff(SpotValues);
        gatherer.DumpOneResult(discounting * thisPayoff);
     }
    return;
}
