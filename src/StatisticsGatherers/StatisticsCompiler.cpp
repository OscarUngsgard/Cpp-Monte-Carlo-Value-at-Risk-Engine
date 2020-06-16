#include "StatisticsCompiler.h"

StatisticsCompiler::StatisticsCompiler(std::vector<Wrapper<StatisticsMC>> Inner_) : Inner(std::move(Inner_))
{
}

StatisticsMC* StatisticsCompiler::clone() const
{
	return new StatisticsCompiler(*this);
}

void StatisticsCompiler::DumpOneResult(double result)
{
	for (unsigned long i = 0; i < Inner.size(); i++)
	{
		Inner[i]->DumpOneResult(result);
	}
}

std::vector<std::vector<double>> StatisticsCompiler::GetResultsSoFar() const
{
	std::vector<std::vector<double>> tmp;
	for (unsigned long i = 0; i < Inner.size(); i++)
	{
		std::vector<std::vector<double>> thisResult(Inner[i]->GetResultsSoFar());
		for (unsigned long j = 0; j < thisResult.size(); j++)
		{
			tmp.push_back(thisResult[j]);
		}
	}
	return tmp;
}