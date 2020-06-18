#include "..\include\StatisticsMean.h"
StatisticsMean::StatisticsMean() : RunningSum(0.0), PathsDone(0)
{
}
void StatisticsMean::DumpOneResult(double result)
{
	RunningSum += result;
	PathsDone++;
}
std::vector<std::vector<double>> StatisticsMean::GetResultsSoFar() const
{
	std::vector<double> innerVector(1);
	std::vector<std::vector<double> > Results(1, innerVector);
	Results[0][0] = RunningSum / PathsDone;
	return Results;
}
StatisticsMC* StatisticsMean::clone() const
{
	return new StatisticsMean(*this);
}