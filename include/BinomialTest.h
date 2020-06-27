#pragma once
#define BINOMIAL_TEST_H
#include <utility>
double BinomProbability(unsigned long numberOfSuccesses, double p, unsigned long trials);
double CumulativeBinomProbability(unsigned long lowerBound, unsigned long upperBound, double p, unsigned long trials);
std::pair<unsigned long, unsigned long> BinomialConfidenceInterval(double p, unsigned long trials, double alpha);

