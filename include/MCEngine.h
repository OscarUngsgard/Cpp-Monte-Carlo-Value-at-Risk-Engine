#pragma once
#define MCENGINE_H
#include "Wrapper.h"
#include "MCStatistics.h"
#include "InstrumentStatisticsGatherer.h"
#include <vector>
#include "Arrays.h"
#include "SimulationEngine.h"
#include "valuationFunction.h"
#include <string>

class MCEngine
{
public:
	MCEngine(std::vector<Wrapper<SimulationEngine>> EngineVector, std::vector<std::vector<double>> covMatrix_);
	void DoSimulation(StatisticsMC& PortfolioStatisticsGatherer, InstrumentStatisticsGatherer& InstrumentStatisticsGatherer, unsigned long numberOfPaths, double timeHorizon);
	void ValuePortfolio();
	double GetPortfolioValue() const;
	MJArray GetInstrumentValues() const;
	std::vector<std::string> GetInstrumentIdentifiers() const;
	std::vector<int> GetInstrumentNominals() const;
	void UpdateTTM(double timeHorizon);
	virtual ~MCEngine() {}
private:
	double V;
	std::vector<Wrapper<SimulationEngine>> EngineVector;
	std::vector<std::vector<double>> covMatrix;
	std::vector<std::vector<double>> cholMatrix;
	std::vector<std::string> functionIdentifiers;
	std::vector<std::reference_wrapper<valuationFunction>> functionReferences;
};
