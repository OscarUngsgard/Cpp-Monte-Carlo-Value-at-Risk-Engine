#include "Random.h"
#include <cstdlib>
#include <cmath>
#include "Cholezky.h"
// the basic math functions should be in namespace
// std but aren’t in VCPP6
#if !defined(_MSC_VER)
using namespace std;
#endif
double GetOneGaussianBySummation()
{
	double result = 0;
	for (unsigned long j = 0; j < 12; j++)
		result += rand() / static_cast<double>(RAND_MAX);
	result -= 6.0;
	return result;
}
double GetOneGaussianByBoxMuller()
{
	double result;
	double x;
	double y;
	double sizeSquared;
	do
	{
		x = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		y = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		sizeSquared = x * x + y * y;
	} while
		(sizeSquared >= 1.0);
		result = x * sqrt(-2 * log(sizeSquared) / sizeSquared);
	return result;
}

MJArray GetArrayOfGauassiansByBoxMuller(unsigned long numberOfVariates)
{
	MJArray normVariateArray(numberOfVariates);
	for (unsigned long i = 0; i < numberOfVariates; i++)
	{
		normVariateArray[i] = GetOneGaussianByBoxMuller();
	}
	return normVariateArray;
}

std::vector<MJArray> GetArraysOfCorrelatedGauassiansByBoxMuller(unsigned long numberOfVariates, std::vector<std::vector<double>> covMatrix)
{
	//Calculate the cholezky Matrix
	std::vector<std::vector<double>> cholezkyMatrix = Cholesky_Decomposition(covMatrix);
	//Fix the size of the arrays to contain correlated normal variates
	std::vector<MJArray> corrNormVariatesVector(cholezkyMatrix.size());
	for (unsigned long j = 0; j < corrNormVariatesVector.size(); j++) {
		corrNormVariatesVector[j].resize(numberOfVariates);
		corrNormVariatesVector[j] = 0;
	}
	//calculate correlated normal variates and fill the arrays with values
	MJArray NormVariates(cholezkyMatrix.size());
	for (unsigned long k = 0; k < numberOfVariates; k++) {
		for (unsigned long i = 0; i < cholezkyMatrix.size(); i++)
		{
			NormVariates[i] = GetOneGaussianByBoxMuller();
			for (unsigned long j = 0; j < cholezkyMatrix[i].size(); j++) {
				corrNormVariatesVector[i][k] += cholezkyMatrix[i][j] * NormVariates[j];
			}
			corrNormVariatesVector[i][k] /= cholezkyMatrix[i][i]; //normalize the random variates
		}
	}
	return corrNormVariatesVector;
}


double CumulatedNormalDist(double x)
{
        // constants
        double a1 = 0.254829592;
        double a2 = -0.284496736;
        double a3 = 1.421413741;
        double a4 = -1.453152027;
        double a5 = 1.061405429;
        double p = 0.3275911;

        // Save the sign of x
        int sign = 1;
        if (x < 0)
            sign = -1;
        x = fabs(x) / sqrt(2.0);

        // A&S formula 7.1.26
        double t = 1.0 / (1.0 + p * x);
        double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * exp(-x * x);

        return 0.5 * (1.0 + sign * y); 
}
