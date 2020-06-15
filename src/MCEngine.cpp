#include "MCEngine.h"
#include "Cholezky.h"
#include "Random.h"
#include <string>
#include <iostream>
#include <chrono>

MCEngine::MCEngine(std::vector<Wrapper<SimulationEngine>> EngineVector_, std::vector<std::vector<double>> covMatrix_)
	: covMatrix(covMatrix_), cholMatrix(Cholesky_Decomposition(covMatrix_)), EngineVector(EngineVector_), V(0)
{
	//Includes all the unique functions in the portfolio to be valued.
	for (unsigned long i = 0; i < EngineVector.size(); ++i)
	{
		std::vector<std::string> thisUniqueIdentifierVector;
		thisUniqueIdentifierVector = EngineVector[i]->GetFunction()->GetuniqueIdentifier();
		std::vector<std::reference_wrapper<valuationFunction>> thisFunctionReferences;
		thisFunctionReferences = EngineVector[i]->GetFunction()->GetInnerReference();
		for (unsigned long j = 0; j < thisUniqueIdentifierVector.size(); ++j)
		{
			if (std::find(functionIdentifiers.begin(), functionIdentifiers.end(), thisUniqueIdentifierVector[j]) == functionIdentifiers.end()) //Checks if unique instrument has been added already.
			{
				functionIdentifiers.push_back(thisUniqueIdentifierVector[j]);	//Stores the unique names of the positions.
				functionReferences.push_back(thisFunctionReferences[j]);		//Stores the actual reference to the position (valuationFunction) that will be accessed to value the portfolio.
			}
		}
	}
}

void MCEngine::DoSimulation(StatisticsMC& PortfolioStatisticsGatherer, InstrumentStatisticsGatherer& InstrumentStatisticsGatherer, unsigned long numberOfPaths, double timeHorizon)
{
	std::chrono::steady_clock sc;
	auto start = sc.now();
	UpdateTTM(timeHorizon);
	double thisPortfolioValue;
	MJArray theseInstrumentValues;
	std::vector<MJArray> correlatedNormVariates = GetArraysOfCorrelatedGauassiansByBoxMuller(numberOfPaths, covMatrix);
	for (unsigned long i = 0; i < numberOfPaths; ++i)
	{
		for (unsigned long j = 0; j < EngineVector.size(); ++j)
		{
			EngineVector[j]->DoOnePath(timeHorizon, sqrt(covMatrix[j][j]) , correlatedNormVariates[j][i]);
		}		
		ValuePortfolio();
		thisPortfolioValue = GetPortfolioValue();
		PortfolioStatisticsGatherer.DumpOneResult(thisPortfolioValue);
		theseInstrumentValues = GetInstrumentValues();
		InstrumentStatisticsGatherer.DumpOneSetOfResult(theseInstrumentValues);
		for (unsigned long j = 0; j < EngineVector.size(); ++j)
		{
			EngineVector[j]->UnDoOnePath(timeHorizon, sqrt(covMatrix[j][j]), correlatedNormVariates[j][i]); //resets the riskfactors for the positions.
		}

		if (i == (numberOfPaths/10) -1)
		{
			auto time_span = static_cast<std::chrono::duration<double>>(sc.now() - start);
			std::cout << "Estimated reamining computation time: " << (1 / 60.0) * time_span.count() * (((double)numberOfPaths-i) / i) << "minutes." << "\n";
			std::cout << "% Completed:";
		}
		if (((i+1) % (numberOfPaths / 10) == 0))
			std::cout << 100 * ((i+1UL) / (double)numberOfPaths) << " "; //progress bar

	}
}

void MCEngine::ValuePortfolio()
{
	V = 0;
	for (unsigned long i = 0; i < functionReferences.size(); ++i)
	{
		functionReferences[i].get().ValueInstrument();
		V += functionReferences[i].get().GetValue();
	}
	return;
}

const double MCEngine::GetPortfolioValue()
{
	return V;
}

const MJArray MCEngine::GetInstrumentValues()
{
	MJArray instrumentValues(functionReferences.size());
	for (unsigned long i = 0; i < functionReferences.size(); ++i)
	{
		instrumentValues[i] = functionReferences[i].get().GetValue();
	}
	return instrumentValues;
}

const std::vector<std::string> MCEngine::GetInstrumentIdentifiers()
{
	return functionIdentifiers;
}

const std::vector<int> MCEngine::GetInstrumentNominals()
{
	std::vector<int> instrumentNominals(functionReferences.size());
	for (unsigned long i = 0; i < functionReferences.size(); ++i)
	{
		instrumentNominals[i] = functionReferences[i].get().GetNominal();
	}
	return instrumentNominals;
}

void MCEngine::UpdateTTM(double timeHorizon)
{
	for (unsigned long i = 0; i < functionReferences.size(); ++i)
	{
		functionReferences[i].get().UpdateTTM(timeHorizon);
	}
	return;
}
