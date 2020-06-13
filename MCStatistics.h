#pragma once
#define STATISTICS_H
#include <vector>
#include "Arrays.h"
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

class StatisticAllPaths : public StatisticsMC
{
public:
	StatisticAllPaths(const unsigned long minimumNumberOfPaths = 1);
	virtual void DumpOneResult(double result);
	virtual std::vector<std::vector<double>> GetResultsSoFar() const;
	const double &GetOneValueFromResultsSoFar(unsigned long index) const;
	virtual StatisticsMC* clone() const;
private:
	std::vector<double> ResultList;
	unsigned long PathsDone;
};

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

class StatisticsAbsVaR : public StatisticsMC
{
public:
	StatisticsAbsVaR(double p_, unsigned long minimumNumberOfPaths = 1);
	virtual void DumpOneResult(double result);
	virtual std::vector<std::vector<double>> GetResultsSoFar() const;
	virtual StatisticsMC* clone() const;
private:
	std::vector<double> ResultList;
	double p;
	unsigned long PathsDone;
};



class StatisticsRelVaR : public StatisticsMC
{
public:
	StatisticsRelVaR(double p_, unsigned long minimumNumberOfPaths = 1);
	virtual void DumpOneResult(double result);
	virtual std::vector<std::vector<double>> GetResultsSoFar() const;
	virtual StatisticsMC* clone() const;
private:
	std::vector<double> ResultList;
	double p;
	unsigned long PathsDone;
};

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
