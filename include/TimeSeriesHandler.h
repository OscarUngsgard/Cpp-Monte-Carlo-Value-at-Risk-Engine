#pragma once
#define TIME_SERIES_HANDLER_H
#include <vector>
#include "AbsOrRelReturns.h"
class TimeSeriesHandler
{
public:
	TimeSeriesHandler(std::vector<std::vector<double>> riskFactorTimeSeries_, std::vector<AbsOrRel> absOrRelReturnsVector_, unsigned long daysBackUsed_ = 252);
	std::vector<double> GetMostRecentValues(unsigned long startingDaysBack = 0);
	std::vector<std::vector<double>> GetReturns();
	void transformToReturns();
	double compute_covariance(std::vector<std::vector<double>> d, int i, int j);
	void createCovarianceMatrix(unsigned long startingDaysBack = 0);
	std::vector<std::vector<double>> GetCovarianceMatrix();
	std::vector<std::vector<double>> GetPartsOfCovarianceMatrix(std::vector<unsigned long> diagonalIndices);
private:
	bool transformedToReturns;
	std::vector<std::vector<double>> riskFactorTimeSeries;
	std::vector<AbsOrRel> absOrRelReturnsVector;
	std::vector<std::vector<double>> returns;
	std::vector<std::vector<double>> covMatrix;
	unsigned long daysBackUsed;
};