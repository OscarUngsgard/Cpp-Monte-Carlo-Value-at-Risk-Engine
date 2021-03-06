#include "BinomialTest.h"
#include "Combinatorics.h"
#include <iostream>
double BinomProbability(unsigned long numberOfSuccesses, double p, unsigned long trials)
{
	return binomCoefficient(trials, numberOfSuccesses) * pow(p, numberOfSuccesses) * pow(1 - p, trials - numberOfSuccesses);
}

double CumulativeBinomProbability(unsigned long lowerBound, unsigned long upperBound, double p, unsigned long trials)
{
	double cumProbability = 0;
	for (unsigned long i = lowerBound; i < upperBound + 1; i++)
	{
		cumProbability += BinomProbability(i, p, trials);
	}
	return cumProbability;
}

std::pair<unsigned long, unsigned long> BinomialConfidenceInterval(double p, unsigned long trials, double alpha)
{
	unsigned long LowerBound = 0;
	unsigned long UpperBound = trials;
	unsigned long currentLowerBound = LowerBound;
	unsigned long currentUpperBound = UpperBound;
	bool lowerUpper;
	while (CumulativeBinomProbability(currentLowerBound, currentUpperBound, p, trials) > (1-alpha))
	{
		if (CumulativeBinomProbability(LowerBound, currentLowerBound + 1, p, trials) > CumulativeBinomProbability(currentUpperBound - 1, UpperBound, p, trials))
		{
			currentUpperBound--;
			lowerUpper = true;
		}
		else
		{ 
			currentLowerBound++;
			lowerUpper = false;
		}
	}
	//Reverse last increment to stay within prescribed confidence interval
	if (lowerUpper)
		currentUpperBound++;
	else
		currentLowerBound--;
	std::pair<unsigned long, unsigned long> interval{ currentLowerBound , currentUpperBound };
	return interval;
}

double BinomTestTwoSidedPValue(double p, unsigned long trials, unsigned long numberOfSuccesses)
{
	//Explaination: https://stats.stackexchange.com/questions/284641/computing-a-two-sided-p-value-when-the-binomial-dist-is-not-symmetric/284666
	double pValue = 0;
	for (unsigned long i = 0; i < trials + 1; i++)
	{
		if (BinomProbability(i, p, trials) <= BinomProbability(numberOfSuccesses, p, trials))
			pValue += BinomProbability(i, p, trials);
	}
	return pValue;
}
