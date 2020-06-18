#include "..\include\StatisticsAbsVaR.h"
#include <algorithm>
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
	std::vector<double> innerVector(1);
	std::vector<std::vector<double> > Results(1, innerVector);

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
