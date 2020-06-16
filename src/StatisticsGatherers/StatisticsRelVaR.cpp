#include "StatisticsRelVaR.h"
#include <algorithm>

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
	std::vector<double> innerVector(1);
	std::vector<std::vector<double> > Results(1, innerVector);

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