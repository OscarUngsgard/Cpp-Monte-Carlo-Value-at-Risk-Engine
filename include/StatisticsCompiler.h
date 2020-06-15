#pragma once
#define STATISTICS_COMPILER_H
#include "MCStatistics.h"
#include "Wrapper.h"


class StatisticsCompiler : public StatisticsMC
{
public:
	StatisticsCompiler(std::vector <Wrapper<StatisticsMC>> Inner_);
	virtual StatisticsMC* clone() const;
	virtual void DumpOneResult(double result);
	virtual std::vector<std::vector<double>> GetResultsSoFar() const;
private:
	std::vector <Wrapper<StatisticsMC>> Inner;
};
