#include "BackTest.h"
#include "MCEngine.h"
#include "InstrumentStatisticsGatherer.h"
#include "BinomialTest.h"

#include <iostream>
#include <chrono>
/// Backtestin functionality - Work in progress
BackTest::BackTest(std::vector<Wrapper<SimulationEngine>> EngineVector_, TimeSeriesHandler timeSeriesHandler_) : EngineVector(EngineVector_), timeSeriesHandler(timeSeriesHandler_), numberOfExceedances(0), currentDaysBack(0), backTestedDays(0)
{
}

void BackTest::RunBackTest(double confidenceLevel, double timeHorizon, unsigned long numberOfPaths, unsigned long daysBack, double alpha)
{
	backTestResults.resize(daysBack);
	std::chrono::steady_clock sc;
	auto start = sc.now();
	for (unsigned long i = 0; i < daysBack; i++)
	{
		backTestResults[i].push_back(currentDaysBack); //First element in results contains index of days back
		StatisticsRelVaR thisRelVarGatherer(confidenceLevel, numberOfPaths);
		InstrumentStatisticsGatherer instrumentGatherer(thisRelVarGatherer, 0);
		timeSeriesHandler.createCovarianceMatrix(currentDaysBack);
		std::vector<std::vector<double>> thisCovMat = timeSeriesHandler.GetCovarianceMatrix();
		MCEngine thisVarEngine(EngineVector, thisCovMat);
		thisVarEngine.ValuePortfolio();
		backTestResults[i].push_back(thisVarEngine.GetPortfolioValue()); //Second element in result contains portfolio value at day
		std::cout.setstate(std::ios_base::failbit); //Supress output of DoSimulation function
		thisVarEngine.DoSimulation(thisRelVarGatherer, instrumentGatherer, numberOfPaths, timeHorizon);
		std::cout.clear(); //Show output again	
		backTestResults[i].push_back(thisRelVarGatherer.GetResultsSoFar()[0][0]); //third element in result contains value at risk at day

		MoveBackOneDay(currentDaysBack);	
		currentDaysBack++;
		if (i == (daysBack / 10) - 1)
		{
			auto time_span = static_cast<std::chrono::duration<double>>(sc.now() - start);
			std::cout << "Estimated reamining computation time: " << (1 / 60.0) * time_span.count() * (((double)daysBack - i) / i) << "minutes." << "\n";
			std::cout << "% Completed:";
		}
		if (daysBack>9)
		{ 
			if (((i + 1) % (daysBack / 10) == 0))
				std::cout << 100 * ((i + 1UL) / (double)daysBack) << " "; //progress bar
		}
	}


	double timeHorizonDays = timeHorizon * 252.0;//Transform to days
	for (unsigned long i = 0; i < timeHorizonDays; i++)
	{
		backTestResults[i].push_back(0); //placeholder for current day realized loss
		backTestResults[i].push_back(0); //placeholder for current day number of exceedances
	}
	for (unsigned long i = timeHorizonDays; i < daysBack; i++)
	{

		double realizedLoss = (backTestResults[i][1] - backTestResults[i - timeHorizonDays][1]) / backTestResults[i][1];
		backTestResults[i].push_back(realizedLoss); //Fourth element in result contains realized loss at day
		backTestResults[i].push_back(backTestResults[i][3] > backTestResults[i][2]); //Fifth element in result is 1 if exceedance, 0 otherwise
		if (backTestResults[i][3] > backTestResults[i][2])
			numberOfExceedances++;
		backTestedDays++;
	}	
	binomConfInterval = BinomialConfidenceInterval((1 - confidenceLevel), backTestedDays, alpha);
}

void BackTest::MoveBackOneDay(unsigned long currentDaysBack)
{
	std::vector<double> theseReturns = timeSeriesHandler.GetReturns()[timeSeriesHandler.GetReturns().size() - 1 - currentDaysBack];
	for (unsigned long i = 0; i < EngineVector.size(); i++)
	{
		EngineVector[i]->ApplyInverseReturns(theseReturns[i]);
	}
}

std::tuple<std::vector<double>, std::vector<std::vector<double>>, std::pair<unsigned long, unsigned long>> BackTest::GetResultsSoFar()  const //std::vector<std::vector<std::vector<double>>>
{
	std::vector<double> results{(double)backTestedDays, (double)numberOfExceedances, numberOfExceedances / (double)backTestedDays };
	std::tuple<std::vector<double>, std::vector<std::vector<double>>, std::pair<unsigned long, unsigned long>> allResults{ results , backTestResults, binomConfInterval };
	return allResults;
}


