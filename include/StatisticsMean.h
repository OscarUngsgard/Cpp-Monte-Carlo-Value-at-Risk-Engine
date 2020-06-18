#pragma once
#define MC_STATISTICS_MEAN_H
#include <vector>
#include "..\include\Arrays.h"
#include <numeric>  
#include "..\include\MCStatistics.h"
class StatisticsMean : public StatisticsMC
{
public:
	StatisticsMean();
	virtual void DumpOneResult(double result);
	virtual std::vector<std::vector<double>> GetResultsSoFar() const;
	virtual StatisticsMC* clone() const;
private:
	double RunningSum;
	unsigned long PathsDone;
};