#pragma once
#define STATISTICS_H
#include <vector>
#include "..\include\Arrays.h"
#include <numeric>  
class StatisticsMC
{
public:
	StatisticsMC() {}
	virtual void DumpOneResult(double result) = 0;
	virtual std::vector<std::vector<double>> GetResultsSoFar() const = 0;
	virtual StatisticsMC* clone() const = 0;
	virtual ~StatisticsMC() {}
private:
};