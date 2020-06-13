#pragma once
#define STATISTIC_H
#include <vector>
#include "Arrays.h"

class Statistics
{
public:
	Statistics() {}
	virtual void DumpOnePath(MJArray results) = 0;
	virtual std::vector<std::vector<double>> GetResultsSoFar() const = 0;
	virtual Statistics* clone() const = 0;
	virtual ~Statistics() {}
private:
};

class StatisticsMean
{
public:
	StatisticsMean() {}
	virtual void DumpOnePath(MJArray results) ;
	virtual std::vector<std::vector<double>> GetResultsSoFar() const ;
	virtual Statistics* clone() const;
private:
	MJArray RunningSums;
	unsigned long PathsDone;
};