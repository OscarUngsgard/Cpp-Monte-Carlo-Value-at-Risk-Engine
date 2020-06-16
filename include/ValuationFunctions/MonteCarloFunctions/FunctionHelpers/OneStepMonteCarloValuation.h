#pragma once
#define SIMPLEMC_H
#include "StandardExcerciseOption.h"
#include "MCStatistics.h"
#include "Arrays.h"
void OneStepMonteCarloValuation(const StandardExcerciseOption& TheOption, double Spot, double Vol, double r, double d, unsigned long NumberOfPaths, MJArray normVariates, StatisticsMC& gatherer);