#pragma once
#define DISCRETIZED_PATH_MC_VALUATION
#include "PathDependantOption.h"
#include "MCStatistics.h"
#include "Arrays.h"
void DiscretizedPathMonteCarloValuation(const PathDependantOption& TheOption, double Spot, double Vol, double r, double d, unsigned long NumberOfPaths, unsigned long NumberOfSteps, MJArray normVariates, StatisticsMC& gatherer);