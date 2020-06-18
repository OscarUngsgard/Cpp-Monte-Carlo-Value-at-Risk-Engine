#include "StatisticsQuantiles.h"
#include <algorithm>
StatisticsQuantiles::StatisticsQuantiles(double p_, unsigned long minimumNumberOfPaths) : p(p_), PathsDone(0)
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
	std::vector<double> innerVector(2);
	std::vector<std::vector<double> > Results(1, innerVector);

	std::vector<double> tmpList(ResultList);
	sort(begin(tmpList), end(tmpList));
	double RunningLowerQunatile = tmpList[floor((1 - p) * PathsDone)];
	double RunningUpperQunatile = tmpList[floor((p)*PathsDone)];
	Results[0][0] = RunningLowerQunatile;
	Results[0][1] = RunningUpperQunatile;
	return Results;
}
StatisticsMC* StatisticsQuantiles::clone() const
{
	return new StatisticsQuantiles(*this);
}
