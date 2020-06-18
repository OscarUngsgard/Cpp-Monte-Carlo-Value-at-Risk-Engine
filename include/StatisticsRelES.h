#pragma once


#pragma once
#define MC_STATISTICS_REL_ES_H
#include <vector>
#include "..\include\Arrays.h"
#include <numeric>  
#include "..\include\MCStatistics.h"
class StatisticsRelES : public StatisticsMC
{
public:
	StatisticsRelES(double p_, unsigned long minimumNumberOfPaths = 1);
	virtual void DumpOneResult(double result);
	virtual std::vector<std::vector<double>> GetResultsSoFar() const;
	virtual StatisticsMC* clone() const;
private:
	std::vector<double> ResultList;
	double p;
	unsigned long PathsDone;
};