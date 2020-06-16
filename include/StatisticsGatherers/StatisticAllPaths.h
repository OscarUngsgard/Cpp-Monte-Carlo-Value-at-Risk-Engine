#pragma once
#define MC_STATISTICS_ALL_PATHS_H
#include <vector>
#include "Arrays.h"
#include <numeric>  
#include "MCStatistics.h"
class StatisticAllPaths : public StatisticsMC
{
public:
	StatisticAllPaths(const unsigned long minimumNumberOfPaths = 1);
	virtual void DumpOneResult(double result);
	virtual std::vector<std::vector<double>> GetResultsSoFar() const;
	const double& GetOneValueFromResultsSoFar(unsigned long index) const;
	virtual StatisticsMC* clone() const;
private:
	std::vector<double> ResultList;
	unsigned long PathsDone;
};