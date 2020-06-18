#pragma once
#define MC_STATISTICS_QUANTILES_H
#include <vector>
#include "Arrays.h"
#include <numeric>  
#include "MCStatistics.h"
class StatisticsQuantiles : public StatisticsMC
{
public:
	StatisticsQuantiles(double p_, const unsigned long minimumNumberOfPaths = 1);
	virtual void DumpOneResult(double result);
	virtual std::vector<std::vector<double>> GetResultsSoFar() const;
	virtual StatisticsMC* clone() const;
private:
	double p;
	std::vector<double> ResultList;
	unsigned long PathsDone;
};