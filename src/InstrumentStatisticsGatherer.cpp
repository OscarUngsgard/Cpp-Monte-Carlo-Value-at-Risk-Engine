#include "InstrumentStatisticsGatherer.h"

InstrumentStatisticsGatherer::InstrumentStatisticsGatherer(Wrapper<StatisticsMC> StatisticsGatherer, unsigned long numberOfInstruments_) : numberOfInstruments(numberOfInstruments_)
{
	for (unsigned long i = 0; i < numberOfInstruments; ++i)
	{
		StatisticsGathererVector.push_back(StatisticsGatherer);
	}
}

void InstrumentStatisticsGatherer::DumpOneSetOfResult(MJArray result)
{
	for (unsigned long i = 0; i < numberOfInstruments; ++i)
	{
		StatisticsGathererVector[i]->DumpOneResult(result[i]);
	}
}

std::vector < std::vector<std::vector<double>>> InstrumentStatisticsGatherer::GetResultsSoFar() const
{
	std::vector<std::vector<std::vector<double>>> Results(numberOfInstruments);
	for (unsigned long i = 0; i < numberOfInstruments; ++i)
	{
		Results[i] = StatisticsGathererVector[i]->GetResultsSoFar();
	}
	return Results;
}

InstrumentStatisticsGatherer* InstrumentStatisticsGatherer::clone() const
{
	return new InstrumentStatisticsGatherer(*this);
}
