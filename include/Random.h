#pragma once
#define RANDOM1_H
#include "..\include\Arrays.h"
#include <vector>
double GetOneGaussianBySummation();
double GetOneGaussianByBoxMuller();
MJArray GetArrayOfGauassiansByBoxMuller(unsigned long numberOfVariates);
std::vector<MJArray> GetArraysOfCorrelatedGauassiansByBoxMuller(unsigned long numberOfVariates, std::vector<std::vector<double>> covMatrix);
double CumulatedNormalDist(double x);



