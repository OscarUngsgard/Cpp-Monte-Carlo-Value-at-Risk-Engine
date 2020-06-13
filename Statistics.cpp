#include "Statistics.h"
using namespace std;
StatisticsMean::StatisticsMean() : PathsDone(0), RunningSums(0) //maybe need to initialize RunningSums here?
{
}

void StatisticsMean::DumpOnePath(MJArray results)
{
	RunningSums.resize(results.size());
	RunningSums += results;
	PathsDone++;
}

std::vector<std::vector<double>> StatisticsMean::GetResultsSoFar() const
{
	vector<double> innerVector(RunningSums.size());
	vector<vector<double> > Results(1, innerVector);
	for (unsigned long i = 1; i < RunningSums.size(); ++i) {
		Results[0][i] = RunningSums[i] / PathsDone;
	}
	return Results;
}

Statistics* StatisticsMean::clone() const
{
	return new StatisticsMean(*this);
}

//Statistics* StatisticsMean::clone() const
//{
//	return new StatisticsMean(*this);
//}
