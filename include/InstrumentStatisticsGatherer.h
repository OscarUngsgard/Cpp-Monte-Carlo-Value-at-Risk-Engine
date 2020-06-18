#pragma once
#define INSTRUMENT_STATISTICS_GATHERER_H
#include <vector>
#include "..\include\Arrays.h"
#include "..\include\MCStatistics.h"
#include "..\include\Wrapper.h"
class InstrumentStatisticsGatherer
{
public:
	InstrumentStatisticsGatherer(Wrapper<StatisticsMC> StatisticsGatherer, unsigned long numberOfInstruments_);
	virtual void DumpOneSetOfResult(MJArray result);
	virtual std::vector < std::vector<std::vector<double>>> GetResultsSoFar() const;
	virtual InstrumentStatisticsGatherer* clone() const;
	virtual ~InstrumentStatisticsGatherer() {}
private:
	std::vector<Wrapper<StatisticsMC>> StatisticsGathererVector;
	unsigned long numberOfInstruments;
};