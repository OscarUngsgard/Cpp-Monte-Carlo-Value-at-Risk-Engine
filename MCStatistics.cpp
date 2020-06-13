#include "MCStatistics.h"
#include <algorithm>
#include <iostream>
using namespace std;

StatisticAllPaths::StatisticAllPaths(const unsigned long minimumNumberOfPaths) : PathsDone(0)
{
	ResultList.reserve(minimumNumberOfPaths);
}

void StatisticAllPaths::DumpOneResult(double result)
{
	ResultList.push_back(result);
	PathsDone++;
}

std::vector<std::vector<double>> StatisticAllPaths::GetResultsSoFar() const
{
	vector<vector<double> > Results;
	Results.push_back(ResultList);
	return Results;
}

const double &StatisticAllPaths::GetOneValueFromResultsSoFar(unsigned long index) const
{
	return ResultList[index];
}


StatisticsMC* StatisticAllPaths::clone() const
{
	return new StatisticAllPaths(*this);
}



StatisticsMean::StatisticsMean() : RunningSum(0.0),PathsDone(0)
{
}
void StatisticsMean::DumpOneResult(double result)
{
	RunningSum += result;
	PathsDone++;
}
vector<vector<double>> StatisticsMean::GetResultsSoFar() const
{
	vector<double> innerVector(1);
	vector<vector<double> > Results(1, innerVector);
	Results[0][0] = RunningSum / PathsDone;
	return Results;
}
StatisticsMC* StatisticsMean::clone() const
{
	return new StatisticsMean(*this);
}


StatisticsQuantiles::StatisticsQuantiles(double p_, unsigned long minimumNumberOfPaths): p(p_), PathsDone(0)
{
	ResultList.reserve(minimumNumberOfPaths);
}

void StatisticsQuantiles::DumpOneResult(double result)
{
	ResultList.push_back(result);
	PathsDone++;
}

std::vector<std::vector<double>> StatisticsQuantiles::GetResultsSoFar() const
{	
	vector<double> innerVector(2);
	vector<vector<double> > Results(1, innerVector);

	std::vector<double> tmpList(ResultList);
	sort(begin(tmpList), end(tmpList));
	double RunningLowerQunatile = tmpList[floor((1-p) * PathsDone)];
	double RunningUpperQunatile = tmpList[floor((p) * PathsDone)];
	Results[0][0] = RunningLowerQunatile;
	Results[0][1] = RunningUpperQunatile;
	return Results;
}
StatisticsMC* StatisticsQuantiles::clone() const
{
	return new StatisticsQuantiles(*this);
}



StatisticsRelVaR::StatisticsRelVaR(double p_, unsigned long minimumNumberOfPaths) : p(p_), PathsDone(0)
{
	ResultList.reserve(minimumNumberOfPaths);
}

void StatisticsRelVaR::DumpOneResult(double result)
{
	ResultList.push_back(result);
	PathsDone++;
}

std::vector<std::vector<double>> StatisticsRelVaR::GetResultsSoFar() const
{
	vector<double> innerVector(1);
	vector<vector<double> > Results(1, innerVector);

	std::vector<double> tmpList(ResultList);
	sort(begin(tmpList), end(tmpList));
	double RunningLowerQunatile = tmpList[floor((1 - p) * PathsDone)];
	double forwardMean = accumulate(tmpList.begin(), tmpList.end(), 0.0) / tmpList.size();
	Results[0][0] = (forwardMean - RunningLowerQunatile) / forwardMean;
	return Results;
}
StatisticsMC* StatisticsRelVaR::clone() const
{
	return new StatisticsRelVaR(*this);
}



StatisticsAbsVaR::StatisticsAbsVaR(double p_, unsigned long minimumNumberOfPaths) : p(p_), PathsDone(0)
{
	ResultList.reserve(minimumNumberOfPaths);
}

void StatisticsAbsVaR::DumpOneResult(double result)
{
	ResultList.push_back(result);
	PathsDone++;
}

std::vector<std::vector<double>> StatisticsAbsVaR::GetResultsSoFar() const
{
	vector<double> innerVector(1);
	vector<vector<double> > Results(1, innerVector);

	std::vector<double> tmpList(ResultList);
	sort(begin(tmpList), end(tmpList));
	double RunningLowerQunatile = tmpList[floor((1 - p) * PathsDone)];
	double forwardMean = accumulate(tmpList.begin(), tmpList.end(), 0.0) / tmpList.size();
	Results[0][0] = forwardMean - RunningLowerQunatile;
	return Results;
}
StatisticsMC* StatisticsAbsVaR::clone() const
{
	return new StatisticsAbsVaR(*this);
}




StatisticsRelES::StatisticsRelES(double p_, unsigned long minimumNumberOfPaths) : p(p_), PathsDone(0)
{
	ResultList.reserve(minimumNumberOfPaths);
}

void StatisticsRelES::DumpOneResult(double result)
{
	ResultList.push_back(result);
	PathsDone++;
}

std::vector<std::vector<double>> StatisticsRelES::GetResultsSoFar() const
{
	vector<double> innerVector(1);
	vector<vector<double> > Results(1, innerVector);
	double RunningAbsES = 0;
	std::vector<double> tmpList(ResultList);
	sort(begin(tmpList), end(tmpList));
	for (unsigned long i = 0; i < floor((1 - p)*PathsDone); ++i) {
		RunningAbsES += tmpList[i];
	}
	RunningAbsES /= floor((1 - p) * PathsDone);
	double forwardMean = accumulate(tmpList.begin(), tmpList.end(), 0.0) / tmpList.size();
	Results[0][0] = (forwardMean - RunningAbsES) / forwardMean;
	return Results;
}

StatisticsMC* StatisticsRelES::clone() const
{
	return new StatisticsRelES(*this);
}

