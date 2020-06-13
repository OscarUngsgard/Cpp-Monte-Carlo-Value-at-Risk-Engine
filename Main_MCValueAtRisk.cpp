// Montecarlo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//System functions
#include <iostream>
#include <cmath>
#include <chrono>
#include <memory>
//General classes and functions
#include "ReadCSV.h"
//Numerical calculations
#include "TimeSeriesHandler.h"
#include "Random.h"
#include "Cholezky.h"
//enums
#include "RiskFactors.h"
#include "AbsOrRelReturns.h"
#include "RainbowOptionTypes.h"
//Statistics calculations e.g. VaR from quantile
#include "MCStatistics.h"
#include "ConvergenceTable.h"
#include "StatisticsCompiler.h"
#include "InstrumentStatisticsGatherer.h"
// Main monte carlo simulations engine
#include "MCEngine.h"
//Inner simulation Engines. E.g. Geometric brownian motion for stocks and simple brownian motion for short rate
#include "SimulationEngine.h"
#include "OneStepBSEngine.h"
#include "OneStepBrownianMotion.h"
//valuation functions used as input into the simulationengine
#include "valuationFunction.h"
#include "FunctionCombiner.h"
#include "EquityForwardFunction.h"
#include "FXForwardFunction.h"
#include "BSCallFunction.h"
#include "BSPutFunction.h"
#include "AmericanCallFunction.h" //Note: Risk for American options are computed with a full revaluation using a binomial tree and hence will increase the computation time significantly if included in the portfolio
#include "AmericanPutFunction.h"
#include "StockPriceFunction.h"
#include "BondFunction.h"
#include "FRAFunction.h" 
#include "InterestRateSwapFunction.h"
#include "FixedForFixedFXSwapFunction.h"
//monte carlo option pricers
#include "MonteCarloVanillaOptionFunction.h" //can be freely used to simulate any payoff 
#include "MonteCarloRainbowOptionFunction.h" //Note: Monte Carlo valuations increase computation time significantly, obviously, but are the only sensible option for rainbow options.
#include "MonteCarloBasketOptionFunction.h"
//Payoff
#include "PayOff.h"

using namespace std;

int main()
{
    //Start clock and set seed
    chrono::steady_clock sc;   // create an object of `steady_clock` class
    auto start = sc.now(); // start timer
    srand(3);

    //Read in file of prices to get most recent prices and covariance matrix
    vector<vector<double>> myTimeSeries = parse2DCsvFile("riskFactorsnew.csv");
    vector<AbsOrRel> AbsOrRelReturns{ AbsOrRel::rel, AbsOrRel::rel, AbsOrRel::abs, AbsOrRel::rel }; //Specifiy if the relative returns or absolute returns are normally distributed for the time series 
    TimeSeriesHandler myTimeSeriesHandlder(myTimeSeries, AbsOrRelReturns);
    myTimeSeriesHandlder.createCovarianceMatrix();
    vector<vector<double>> myCovMatrix = myTimeSeriesHandlder.GetCovarianceMatrix();
    vector<double> currentPrices = myTimeSeriesHandlder.GetMostRecentValues();
  
    ////output the covariance matrix used  
    //for (unsigned long i = 0; i < myReturns.size(); i++)
    //{
    //    for (unsigned long j = 0; j < myReturns[i].size(); j++)
    //    {
    //        std::cout << myReturns[i][j] << ", ";
    //    }
    //    std::cout << "\n";
    //}
    //std::cout << "\n";
    //for (unsigned long i = 0; i < myCovMatrix.size(); i++)
    //{
    //    for (unsigned long j = 0; j < myCovMatrix[i].size(); j++)
    //    {
    //        std::cout << myCovMatrix[i][j] << ", ";
    //    }
    //    std::cout << "\n";
    //}

    ///Manually creates Covariance matrix and current price///
    //std::vector<double> covmatrow1{ 0.38 * 0.38, 0 * 0.5 * 0.1 * 0.1 };
    //std::vector<double> covmatrow2{ 0* 0.5 * 0.1 * 0.1, 0.006 * 0.006 };
    //std::vector<std::vector<double>> covmat{ covmatrow1, covmatrow2 };
    //currentPrices[0] = 100; currentPrices[1] = 0.01;

    //Declare variables and set some constants
    const unsigned long binomialTreeSteps = 5; //for american options
    const unsigned long NumberOfPaths = pow(10, 4);
    const unsigned long MCValuationNumberOfPaths = pow(10, 3);
    double timeHorizon = 1/252.0;
    double S0;    double TTM;    double Strike;    double d;    double impvol;    double contractRate;    double facevalue;
    double yield;    signed long nominal;    unsigned long couponFreq;    unsigned long freq;    double couponRate;    
    double zeroDrift = 0;    double r = 0.0065;

    //Creating the positions//
    //RiskFactor 1 derivatives
    nominal = 10; S0 = currentPrices[0];
    std::shared_ptr<valuationFunction> stillFrontStock = std::make_shared<StockPriceFunction>("Stillfront", nominal, S0);
    nominal = 100; d = 0; Strike = currentPrices[0]; impvol = 0.5; TTM = 1 / 12.0;
    std::shared_ptr<valuationFunction> stillFrontEuropeanCall = std::make_shared<BSCallFunction>("StillFront ATM 1 month European call", nominal, S0, r, d, impvol, TTM, Strike);
    PayOffCall thisCallPayOff(Strike);
    std::shared_ptr<valuationFunction> stillFrontEuropeanMonteCarloCall = std::make_shared<MonteCarloVanillaOptionFunction>("StillFront ATM 1 month European Monte Carlo call", nominal, S0, r, d, impvol, TTM, thisCallPayOff, MCValuationNumberOfPaths);
    std::shared_ptr<valuationFunction> stillFrontEuropeanPut = std::make_shared<BSPutFunction>("StillFront ATM 1 European month put", nominal, S0, r, d, impvol, TTM, Strike);
    //RiskFactor 2 derivatives
    nominal = -50; S0 = currentPrices[1];
    std::shared_ptr<valuationFunction> storytelStock = std::make_shared<StockPriceFunction>("Storytel", nominal, S0);
    nominal = 100; d = 0; Strike = currentPrices[1]; impvol = 0.5; TTM = 1 / 12.0;
    std::shared_ptr<valuationFunction> StorytelAmericanCall = std::make_shared<AmericanCallFunction>("Storytel ATM 1 month American call", nominal, S0, r, d, impvol, TTM, Strike, binomialTreeSteps);
    std::shared_ptr<valuationFunction> StorytelEuropeanCall = std::make_shared<BSCallFunction>("Storytel ATM 1 month European call", nominal, S0, r, d, impvol, TTM, Strike);
    //RiskFactor 2/3 Derivatives
    vector<vector<double>> rainBowCovMatrix = myTimeSeriesHandlder.GetPartsOfCovarianceMatrix(vector<unsigned long>{0, 1});
    vector<double> S0_vect{ currentPrices[0], currentPrices[1] }; vector<double> impvol_vect{ 0.5, 0.5 }; vector<double> div_vect{ 0.03, 0.02 };
    PayOffCall StillfrontATMCallPayOff(S0_vect[0]); PayOffCall storytelATMCallPayOff(S0_vect[1]);
    vector<Wrapper<PayOff>> rainbowPayoffs{ StillfrontATMCallPayOff, storytelATMCallPayOff };
    std::shared_ptr<valuationFunction> stillFrontStoryTelWorstOfCallOption = std::make_shared<MonteCarloRainbowOptionFunction>("Worst-Of Call Option StilFront StoryTel", nominal, S0_vect, r, div_vect, impvol_vect, rainBowCovMatrix, TTM, rainbowPayoffs, MCValuationNumberOfPaths, RainbowOptionType::worst_of);
    std::shared_ptr<valuationFunction> stillFrontStoryTelBestOfCallOption = std::make_shared<MonteCarloRainbowOptionFunction>("Best-Of Call Option StilFront StoryTel", nominal, S0_vect, r, div_vect, impvol_vect, rainBowCovMatrix, TTM, rainbowPayoffs, MCValuationNumberOfPaths, RainbowOptionType::best_of);
    PayOffCall basketCallPayOff(S0_vect[0] * 0.4 + S0_vect[1] * 0.6); std::vector<double> basketWeights{ 0.4,0.6 };
    std::shared_ptr<valuationFunction> stillFrontStoryTelBasketCallOption = std::make_shared<MonteCarloBasketOptionFunction>("Basket Call Option StilFront StoryTel", nominal, S0_vect, basketWeights, r, div_vect, impvol_vect, rainBowCovMatrix, TTM, basketCallPayOff, MCValuationNumberOfPaths);
    //RiskFactor 3 derivatives
    nominal = 10; yield = currentPrices[2];  facevalue = 100; couponFreq = 2; couponRate = 0.01; TTM = 10;
    std::shared_ptr<valuationFunction> USTreasuryBond = std::make_shared<BondFunction>("10 year US treasury bond", nominal, yield, facevalue, couponRate, couponFreq, TTM);
    nominal = 100000;  MJArray riskFreeRates(3); MJArray forwardRates(3); contractRate = 0.006; freq = 2; TTM = 1.25;
    riskFreeRates[0] = 0.0050; riskFreeRates[1] = 0.0055; riskFreeRates[2] = 0.006; forwardRates[0] = 0.055; forwardRates[1] = 0.0062; forwardRates[2] = 0.007;
    std::shared_ptr<valuationFunction> USDIRS = std::make_shared<InterestRateSwapFunction>("Interest rate swap on US 10 year yield", nominal, r, contractRate, forwardRates, freq, TTM);
    //RiskFactor 4 derivatives
    nominal = 1000; double F0 = 1.15; double r_foreign = -0.003; double FXrate = (currentPrices[3]); TTM = 1;
    std::shared_ptr<valuationFunction> EURUSDForward = std::make_shared<FXForwardFunction>("EUR/USD FX forward", nominal, FXrate, r, r_foreign, TTM, F0);
    double notional_d = 1110000; double notional_f = 1000000; FXrate = (currentPrices[3]); double r_domestic = r; double r_contract_d = 0.04; double r_contract_f = 0.03; TTM = 3; freq = 1;
    std::shared_ptr<valuationFunction> EURUSDFXSwap = std::make_shared<FixedForFixedFXSwapFunction>("EUR/USD FX Swap", notional_d, notional_f, FXrate, r_domestic, r_foreign, r_contract_d, r_contract_f, freq, TTM);

    //Combining the positions into new groups that will be stressed for each risk factor
    std::shared_ptr<valuationFunction> StillFrontFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ stillFrontStock, stillFrontEuropeanCall, stillFrontEuropeanMonteCarloCall, stillFrontStoryTelBestOfCallOption, stillFrontStoryTelWorstOfCallOption, stillFrontStoryTelBasketCallOption }); //Several different instruments are simulated with the process for this risk factor 
    std::shared_ptr<valuationFunction> StorytelFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ storytelStock, StorytelAmericanCall, StorytelEuropeanCall, stillFrontStoryTelBestOfCallOption, stillFrontStoryTelWorstOfCallOption, stillFrontStoryTelBasketCallOption });
    std::shared_ptr<valuationFunction> USTreasuryFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ USTreasuryBond, USDIRS, stillFrontEuropeanCall, StorytelAmericanCall }); //Can add equitiy derivates and others here as well to simulate the risk free rate for discounting
    std::shared_ptr<valuationFunction> USDEURFXFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ EURUSDForward });
    
    //Selecting the stochastic processes and risk factors to simulate for each position
    OneStepBSEngine StockSimulation(timeHorizon, zeroDrift, StillFrontFunctions, RiskFactor::equity1);
    OneStepBSEngine StockSimulation2(timeHorizon, zeroDrift, StorytelFunctions, RiskFactor::equity2);
    OneStepBrownianMotionEngine ShortRateSimulation(timeHorizon, zeroDrift, USTreasuryFunctions, RiskFactor::interest_rate);
    double FXDrift = r_domestic - r_foreign;
    OneStepBSEngine FXSimulation(timeHorizon, FXDrift, USDEURFXFunctions, RiskFactor::FX_rate);
    //create vector of the engines that will be used in MC simulation. NOTE: Make sure to add in same order as covariance matrix was created.
    vector<Wrapper<SimulationEngine>>EngineVector; 
    EngineVector.push_back(StockSimulation);
    EngineVector.push_back(StockSimulation2);
    EngineVector.push_back(ShortRateSimulation);
    EngineVector.push_back(FXSimulation);

    try {
        //Setting up the engine and calculating the present value of the portfolio and positions
        MCEngine VAREngine(EngineVector, myCovMatrix);
        VAREngine.ValuePortfolio();
        double V0 = VAREngine.GetPortfolioValue();
        std::cout << "Initial values: " << "\n";
        std::cout << "Portolio PV = " << V0;
        std::cout << "\n";
        MJArray instrumentValues = VAREngine.GetInstrumentValues();
        std::vector<std::string> instrumentIdentifiers = VAREngine.GetInstrumentIdentifiers();
        for (unsigned long i = 0; i < instrumentValues.size(); i++)
        {
            std::cout << instrumentIdentifiers[i] << " position PV = " << instrumentValues[i] << "\n";
        }
        std::cout << "\n";
        //Creating the statistics gatherers for the simulation
        double p = 0.99;
        StatisticsMean meanGatherer;
        //StatisticsQuantiles absQuantileGatherer(p, NumberOfPaths);
        StatisticsAbsVaR absVarGatherer(p, NumberOfPaths);
        StatisticsRelVaR relVaRGatherer(p, NumberOfPaths);
        StatisticsRelES relESGatherer(p, NumberOfPaths);
        ConvergenceTable ConvergenceTableGatherer(relVaRGatherer);
        StatisticsCompiler gathererCombiner({ meanGatherer, absVarGatherer, relVaRGatherer, relESGatherer, ConvergenceTableGatherer });
        //Creating a gatherer for individual instruments
        unsigned long totalNumberOfInstruments = instrumentIdentifiers.size();
        StatisticsCompiler InstrumentgathererCombiner({ meanGatherer, absVarGatherer, relVaRGatherer, relESGatherer });
        InstrumentStatisticsGatherer instrumentGatherer(InstrumentgathererCombiner, totalNumberOfInstruments);
        //Running the Monte-Carlo Value at Risk simulation
        VAREngine.DoSimulation(gathererCombiner, instrumentGatherer, NumberOfPaths);
        vector<vector<double> > VARresults = gathererCombiner.GetResultsSoFar();
        std::cout << "\n";
        std::cout << "Portfolio results: " << "\n";
        std::cout << "\n";
        std::cout << "forward mean = ";
        for (unsigned long j = 0; j < VARresults[0].size(); j++)
            std::cout << VARresults[0][j] << " " << "\n";
        //std::cout << 1 - p << "," << p << " quantiles = ";
        std::cout << 100 * p << "% absolute VaR = ";
        for (unsigned long j = 0; j < VARresults[1].size(); j++)
            std::cout << VARresults[1][j] << " ";
        std::cout << "\n";
        std::cout << 100 * p << "% relative VaR = ";
        for (unsigned long j = 0; j < VARresults[2].size(); j++)
            std::cout << VARresults[2][j] * 100 << "% " << "\n";
        std::cout << 100 * p << "% relative ES = ";
        for (unsigned long j = 0; j < VARresults[3].size(); j++)
            std::cout << VARresults[3][j] * 100 << "% " << "\n";
        std::cout << 100 * p << "% relative VaR Convergence table: " << "\n";
        for (unsigned long i = 4; i < VARresults.size(); i++) {
            for (unsigned long j = 0; j < VARresults[i].size(); j++)
            {
                std::cout << VARresults[i][j] << " ";
                if (j % 2 != 0)
                    std::cout << " paths done." << "\n";
            }
        }
        std::cout << "\n";
        std::cout << "Instrument results: " << "\n";
        std::cout << "\n";
        vector<vector<vector<double>>> InstrumentVARresults = instrumentGatherer.GetResultsSoFar();
        for (unsigned long i = 0; i < InstrumentVARresults.size(); i++)
        {
            std::cout << instrumentIdentifiers[i] << ":" << "\n";
            std::cout << "forward mean = ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][0].size(); j++)
                std::cout << InstrumentVARresults[i][0][j] << " " << "\n";
            std::cout << 100 * p << "% absolute VaR = ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][1].size(); j++)
                std::cout << InstrumentVARresults[i][1][j]<< " ";
            std::cout << "\n";
            std::cout << 100 * p << "% relative VaR = ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][2].size(); j++)
                std::cout << InstrumentVARresults[i][2][j]*100 << "% " << "\n";
            std::cout << 100 * p << "% relative ES = ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][3].size(); j++)
                std::cout << InstrumentVARresults[i][3][j] * 100 << "% " << "\n";
            std::cout << "\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Caught exception: " << e.what() << '\n';
    }

    //Track time and end
    auto end = sc.now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
    auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
    std::cout << "Operation took: " << time_span.count() << " seconds.";
    double tmp;
    std::cin >> tmp;
    return 0;
}