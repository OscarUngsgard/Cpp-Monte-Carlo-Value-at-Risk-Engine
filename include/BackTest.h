#pragma once
#define BACKTEST_H
#include "Wrapper.h"
#include "MCStatistics.h"
#include "InstrumentStatisticsGatherer.h"
#include <vector>
#include "Arrays.h"
#include "SimulationEngine.h"


#include "StatisticsRelVaR.h"
#include "TimeSeriesHandler.h"
/// Backtestin functionality - Work in progress
class BackTest
{
public:
	BackTest(std::vector<Wrapper<SimulationEngine>> EngineVector_,  TimeSeriesHandler timeSeriesHandler_);
	void RunBackTest(double confidenceLevel, double timeHorizon, unsigned long numberOfPaths, unsigned long daysBack, double alpha);
	void MoveBackOneDay(unsigned long currentDaysBack);
	std::tuple<std::vector<double>, std::vector<std::vector<double>>, std::pair<unsigned long, unsigned long>> GetResultsSoFar() const;
	virtual ~BackTest() {}
private:
	std::vector<Wrapper<SimulationEngine>> EngineVector;
	TimeSeriesHandler timeSeriesHandler;
	unsigned long numberOfExceedances;
	unsigned long currentDaysBack;
	unsigned long backTestedDays;
	std::vector<std::vector<double>> backTestResults;
	std::pair<unsigned long, unsigned long> binomConfInterval;
};
