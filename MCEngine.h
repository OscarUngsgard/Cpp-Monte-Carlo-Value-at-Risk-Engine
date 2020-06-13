#pragma once
#define MCENGINE_H
#include "wrapper.h"
#include "MCStatistics.h"
#include "InstrumentStatisticsGatherer.h"
#include <vector>
#include "CashFlow.h"
#include "Arrays.h"
#include "SimulationEngine.h"
#include "valuationFunction.h"
#include "FunctionBridge.h"
#include <string>

class MCEngine
{
public:
	MCEngine(std::vector<Wrapper<SimulationEngine>> EngineVector, std::vector<std::vector<double>> covMatrix_);
	void DoSimulation(StatisticsMC& PortfolioStatisticsGatherer, InstrumentStatisticsGatherer& InstrumentStatisticsGatherer, unsigned long numberOfPaths);
	void ValuePortfolio();
	const double GetPortfolioValue();
	const MJArray GetInstrumentValues();
	const std::vector<std::string> GetInstrumentIdentifiers();
	void UpdateTTM();
	virtual ~MCEngine() {}
private:
	double V;
	std::vector<std::vector<double>> cholMatrix;
	std::vector<std::vector<double>> covMatrix;
	std::vector<Wrapper<SimulationEngine>> EngineVector;
	std::vector<std::string> functionIdentifiers;
	std::vector<std::reference_wrapper<valuationFunction>> functionReferences;
};
