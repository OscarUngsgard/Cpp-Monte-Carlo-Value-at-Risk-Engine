#pragma once
#define MCENGINE_H
#include "..\include\Wrapper.h"
#include "..\include\MCStatistics.h"
#include "..\include\InstrumentStatisticsGatherer.h"
#include <vector>
#include "..\include\Arrays.h"
#include "..\include\SimulationEngine.h"
#include "..\include\valuationFunction.h"
#include <string>

class MCEngine
{
public:
	MCEngine(std::vector<Wrapper<SimulationEngine>> EngineVector, std::vector<std::vector<double>> covMatrix_);
	void DoSimulation(StatisticsMC& PortfolioStatisticsGatherer, InstrumentStatisticsGatherer& InstrumentStatisticsGatherer, unsigned long numberOfPaths, double timeHorizon);
	void ValuePortfolio();
	const double GetPortfolioValue();
	const MJArray GetInstrumentValues();
	const std::vector<std::string> GetInstrumentIdentifiers();
	const std::vector<int> GetInstrumentNominals();
	void UpdateTTM(double timeHorizon);
	virtual ~MCEngine() {}
private:
	double V;
	std::vector<std::vector<double>> cholMatrix;
	std::vector<std::vector<double>> covMatrix;
	std::vector<Wrapper<SimulationEngine>> EngineVector;
	std::vector<std::string> functionIdentifiers;
	std::vector<std::reference_wrapper<valuationFunction>> functionReferences;
};
