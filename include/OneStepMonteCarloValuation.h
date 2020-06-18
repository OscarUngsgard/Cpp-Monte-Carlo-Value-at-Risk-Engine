#pragma once
#define SIMPLEMC_H
#include "..\include\StandardExcerciseOption.h"
#include "..\include\MCStatistics.h"
#include "..\include\Arrays.h"
void OneStepMonteCarloValuation(const StandardExcerciseOption& TheOption, double Spot, double Vol, double r, double d, unsigned long NumberOfPaths, MJArray normVariates, StatisticsMC& gatherer);