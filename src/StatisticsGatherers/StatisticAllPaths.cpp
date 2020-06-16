#include "StatisticAllPaths.h"
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

const double& StatisticAllPaths::GetOneValueFromResultsSoFar(unsigned long index) const
{
	return ResultList[index];
}


StatisticsMC* StatisticAllPaths::clone() const
{
	return new StatisticAllPaths(*this);
}
