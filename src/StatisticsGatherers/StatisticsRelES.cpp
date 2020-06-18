#include "StatisticsRelES.h"
#include <algorithm>
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
	std::vector<double> innerVector(1);
	std::vector<std::vector<double> > Results(1, innerVector);
	double RunningAbsES = 0;
	std::vector<double> tmpList(ResultList);
	sort(begin(tmpList), end(tmpList));
	for (unsigned long i = 0; i < floor((1 - p) * PathsDone); ++i) {
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

