#include "TimeSeriesHandler.h"
#include <iostream>
#include <cmath>
#include <assert.h>
#include <algorithm>
#include <list>

TimeSeriesHandler::TimeSeriesHandler(std::vector<std::vector<double>> riskFactorTimeSeries_, std::vector<AbsOrRel> absOrRelReturnsVector_, unsigned long daysBackUsed_) : riskFactorTimeSeries(riskFactorTimeSeries_), absOrRelReturnsVector(absOrRelReturnsVector_), daysBackUsed(daysBackUsed_)
{
	transformedToReturns = false;
}

std::vector<double> TimeSeriesHandler::GetMostRecentValues(unsigned long startingDaysBack)
{
	std::vector<double> mostRecentValues(riskFactorTimeSeries.back().size());
	for (unsigned long j = 0; j < mostRecentValues.size(); j++)
	{
		mostRecentValues[j] = riskFactorTimeSeries[riskFactorTimeSeries.size()-1-startingDaysBack][j];
	}
	return mostRecentValues;
}

std::vector<std::vector<double>> TimeSeriesHandler::GetReturns()
{
	return returns;
}

void TimeSeriesHandler::transformToReturns()
{
	transformedToReturns = true;
	returns = riskFactorTimeSeries;
	returns.resize(riskFactorTimeSeries.size() - 1);
	for (unsigned long j = 0; j < riskFactorTimeSeries[1].size(); j++)
	{
		switch (absOrRelReturnsVector[j])
		{
		case AbsOrRel::abs:
			for (unsigned long i = 0; i < riskFactorTimeSeries.size()-1; i++)
			{
				returns[i][j] = riskFactorTimeSeries[i+1][j] - riskFactorTimeSeries[i][j];
			}
			break;
		case AbsOrRel::rel:
			for (unsigned long i = 0; i < riskFactorTimeSeries.size()-1; i++)
			{
				returns[i][j] = log(riskFactorTimeSeries[i+1][j]/ riskFactorTimeSeries[i][j]);
			}
			break;
		default:
			break;
		}
	}
	return;
}

double TimeSeriesHandler::compute_covariance(std::vector<std::vector<double>> d, int i, int j) {
	double cov = 0;
	for (unsigned long k = 0; k < d.size(); ++k) {
		cov += d[k][i] * d[k][j];
	}

	return cov / (d.size() - 1);
}

void TimeSeriesHandler::createCovarianceMatrix(unsigned long startingDaysBack)
{
	if (!transformedToReturns)
		transformToReturns();
	if (returns.size() < (daysBackUsed + startingDaysBack))
	{
		daysBackUsed = returns.size() - startingDaysBack;
		std::cout << "Warning: short time series. Only using " << daysBackUsed << " number of days back. \n \n";
	}
	std::vector<std::vector<double>>::const_iterator first = returns.end() - startingDaysBack - daysBackUsed;
	std::vector<std::vector<double>>::const_iterator last = returns.end() - startingDaysBack;
	std::vector<std::vector<double>> reSizedReturns(first, last);

	int dim = reSizedReturns[0].size();
	covMatrix.resize(dim);
	for (int i = 0; i < dim; ++i) {
		covMatrix[i].resize(dim);
		for (int j = i; j < dim; ++j) {
			covMatrix[i][j] = compute_covariance(reSizedReturns, i, j) * 252; //*252 to scale to yearly measurment
		}
	}
	// fill the Left triangular matrix
	for (int i = 1; i < dim; i++) {
		for (int j = 0; j < i; ++j) {
			covMatrix[i][j] = covMatrix[j][i];
		}
	}

}

std::vector<std::vector<double>> TimeSeriesHandler::GetCovarianceMatrix()
{
	return covMatrix;
}

std::vector<std::vector<double>> TimeSeriesHandler::GetPartsOfCovarianceMatrix(std::vector<unsigned long> diagonalIndices)
{
	std::vector<std::vector<double>> smallerCovMatrix;
	smallerCovMatrix.resize(diagonalIndices.size());
	unsigned long smallCovMatrixIndex = 0;
	for (unsigned long i = 0; i < covMatrix.size(); i++) {
		if (std::find(diagonalIndices.begin(), diagonalIndices.end(), i) != diagonalIndices.end())
		{ 
			for (unsigned long j = 0; j < covMatrix.size(); ++j) {
				if (std::find(diagonalIndices.begin(), diagonalIndices.end(), j) != diagonalIndices.end())
				{ 
					smallerCovMatrix[smallCovMatrixIndex].push_back(covMatrix[i][j]);
				}
			}
			smallCovMatrixIndex += 1;
		}
	}
	return smallerCovMatrix;
}
