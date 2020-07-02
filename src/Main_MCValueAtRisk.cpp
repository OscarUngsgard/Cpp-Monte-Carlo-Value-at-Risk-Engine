//System functions
#include <iostream>
#include <cmath>
#include <chrono>
#include <memory>
#include <algorithm>
//General classes and functions
#include "ReadCSV.h"
#include "Wrapper.h"
//Numerical calculations
#include "TimeSeriesHandler.h"
#include "Random.h"
#include "Cholezky.h"
#include "BinomialTest.h"
#include "Combinatorics.h"
//enums
#include "RiskFactors.h"
#include "AbsOrRelReturns.h"
#include "RainbowOptionTypes.h"
//Statistics calculations e.g. VaR from quantile
#include "MCStatistics.h"
#include "StatisticsMean.h"
#include "StatisticsRelVaR.h"
#include "StatisticsAbsVaR.h"
#include "StatisticsRelES.h"
#include "StatisticsQuantiles.h"
#include "ConvergenceTable.h"
#include "StatisticsCompiler.h"
#include "InstrumentStatisticsGatherer.h"
// Main monte carlo VaR simulations engine
#include "MCEngine.h"
//Inner VaR simulation Engines. E.g. Geometric brownian motion for stocks and simple brownian motion for short rate
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
#include "AmericanGeneralPayOffFunction.h" // Note: Risk for American options are computed with a full revaluation using a binomial treeand hence will increase the computation time significantly if included in the portfolio
#include "AmericanCallFunction.h" 
#include "AmericanPutFunction.h"
#include "StockPriceFunction.h"
#include "BondFunction.h"
#include "FRAFunction.h" 
#include "InterestRateSwapFunction.h"
#include "FixedForFixedFXSwapFunction.h"
//monte carlo option pricers
#include "MonteCarloVanillaOptionFunction.h" //can be freely used to simulate any payoff 
#include "MonteCarloRainbowOptionFunction.h" //Note: Monte Carlo valuations increase computation time significantly but are the only sensible alternative for rainbow options
#include "MonteCarloBasketOptionFunction.h"
//Payoffs
#include "PayOffCall.h"
#include "PayOffPut.h"
#include "PayOffCallRelative.h"
#include "PayOffPutRelative.h"
//Backtest
#include "BackTest.h"



using namespace std;

int main()
{
    //Start timer and set seed
    chrono::steady_clock sc;   
    auto start = sc.now();
    srand(1);

    //Declare variables and set some constants

    //Reasonable values for a 250 day backtest is 1000 Value at Risk paths and 5000 MC valuation paths. For just a VaR simulation 10,000 paths and 50,000 valuation paths can easily be used. If just valuing then several millions paths can be used.

    unsigned long riskFactorDaysUsed = 240;
    unsigned long NumberOfPaths;// = pow(10, 3); //For the Monte-Carlo value at risk simmulation
    unsigned long binomialTreeSteps;// = 10; //for american options
    unsigned long MCValuationNumberOfPaths;// = 5 * pow(10, 3); //For Monte-Carlo option valuations of e.g. for Best-of/worst-of options
    double timeHorizon; // = 20.0 / 252.0;
    unsigned long daysBack;// = 20;
    double S0;    double TTM;    double Strike;    double d;    double contractRate;    double facevalue;
    double yield;    signed long nominal;    unsigned long couponFreq;    unsigned long freq;    double couponRate;
    double p; double alpha; //binom test statistic for backtesting
    double r = 0.0035;
    double zeroDrift = 0;

    std::cout << "Input Time horizon (days): "; std::cin >> timeHorizon; timeHorizon /= 252.0;
    std::cout << "Input VaR confidence factor: "; std::cin >> p;
    std::cout << "Input number of paths: ";   std::cin >> NumberOfPaths;   
    std::cout << "Input number of simulations for Monte-Carlo option valuations: "; std::cin >> MCValuationNumberOfPaths; 
    std::cout << "Input number of steps for binomial tree option valuations: "; std::cin >> binomialTreeSteps;
    std::cout << "Input number of days looked back for backtest: "; std::cin >> daysBack; 
    std::cout << "Input signficance level (alpha) used in backtest: "; std::cin >> alpha;
    std::cout << "\n";
    
    //Read in file of prices to get most recent prices and covariance matrix
    vector<vector<double>> myTimeSeries = parse2DCsvFile("RiskFactors.csv");
    vector<AbsOrRel> AbsOrRelReturns{ AbsOrRel::rel, AbsOrRel::rel, AbsOrRel::abs, AbsOrRel::rel, AbsOrRel::rel, AbsOrRel::rel, AbsOrRel::rel }; //Specifiy if the relative returns or absolute returns are normally distributed for the time series 
    TimeSeriesHandler myTimeSeriesHandlder(myTimeSeries, AbsOrRelReturns, riskFactorDaysUsed);
    myTimeSeriesHandlder.createCovarianceMatrix(0);
    vector<vector<double>> myCovMatrix = myTimeSeriesHandlder.GetCovarianceMatrix();
    vector<double> spotRates = myTimeSeriesHandlder.GetMostRecentValues();

    ///output the returns and covariance matrix used

    //vector<vector<double>> myReturns = myTimeSeriesHandlder.GetReturns();
    //for (unsigned long i = 0; i < myReturns.size(); i++)
    //{
    //    for (unsigned long j = 0; j < myReturns[i].size(); j++)
    //    {
    //        std::cout << myReturns[i][j] << ", ";
    //    }
    //    std::cout << "\n";
    //}
    //std::cout << "\n";
    //std::cout << "\n";
    /////output the covariance matrix and spot rates used  
    //vector<double>  currentPrices = myTimeSeriesHandlder.GetMostRecentValues();
    //for (unsigned long i = 0; i < currentPrices.size(); i++)
    //{
    //    std::cout << "Current prices: " << currentPrices[i] << "\n";
    //}
    //
    //for (unsigned long i = 0; i < myCovMatrix.size(); i++)
    //{
    //    for (unsigned long j = 0; j < myCovMatrix[i].size(); j++)
    //    {
    //        std::cout << myCovMatrix[i][j] << ", ";
    //    }
    //    std::cout << "\n";
    //}

    //Creating the positions//
    //RiskFactor 1 derivatives - A butterfly spread
    nominal = 100; d = 0; S0 = spotRates[0]; Strike = 0.85 * spotRates[0]; double stilfrontImpvol = 0.5; TTM =  3.0 / 12.0;
    std::shared_ptr<valuationFunction> stillFrontEuropeanCallButterflySpread1 = std::make_shared<BSCallFunction>("StillFront 3 month ITM European call (butterfly spread)", nominal, S0, r, d, stilfrontImpvol, TTM, Strike);
    nominal = -200; Strike = spotRates[0];
    std::shared_ptr<valuationFunction> stillFrontEuropeanCallButterflySpread2 = std::make_shared<BSCallFunction>("StillFront 3 month ATM European call (butterfly spread)", nominal, S0, r, d, stilfrontImpvol, TTM, Strike);
    nominal = 100;  Strike = 1.15 * spotRates[0];
    std::shared_ptr<valuationFunction> stillFrontEuropeanCallButterflySpread3 = std::make_shared<BSCallFunction>("StillFront 3 month OTM European call (butterfly spread)", nominal, S0, r, d, stilfrontImpvol, TTM, Strike);
    //RiskFactor 2 derivatives - A covered call
    nominal = 140; S0 = spotRates[1];
    std::shared_ptr<valuationFunction> storytelStock = std::make_shared<StockPriceFunction>("Storytel", nominal, S0);
    nominal = -200; double storytelDivYield = 0; Strike = 0.9 * spotRates[1]; double storytelImpvol = 0.5; TTM = 12.0 / 12.0;
    PayOffCall StorytelITMCallPayOff(0.9 * spotRates[1]);
    std::shared_ptr<valuationFunction> StorytelAmericanCall = std::make_shared<AmericanGeneralPayOffFunction>("Storytel 3 month In-the-money American call", nominal, S0, r, storytelDivYield, storytelImpvol, TTM, StorytelITMCallPayOff, binomialTreeSteps);
    // std::shared_ptr<valuationFunction> stillFrontEuropeanPut = std::make_shared<BSPutFunction>("StillFront 3 month ATM European put", nominal, S0, r, d, stilfrontImpvol, TTM, Strike);
    //PayOffPut storytelATMPutPayOff(S0);
    // std::shared_ptr<valuationFunction> stillFrontEuropeanPutMonteCarlo = std::make_shared<MonteCarloVanillaOptionFunction>("StillFront 3 month ATM European put Monte Carlo", nominal, S0, r, d, stilfrontImpvol, TTM, storytelATMPutPayOff, MCValuationNumberOfPaths);
    //RiskFactor 3 derivatives - Derivatives on the US 10 year treasury yield
    nominal = 100; yield = spotRates[2]/100.0;  facevalue = 100; couponFreq = 2; couponRate = 0.01; TTM = 2;
    std::shared_ptr<valuationFunction> USTreasuryBond = std::make_shared<BondFunction>("10 year US treasury bond", nominal, yield, facevalue, couponRate, couponFreq, TTM);
    nominal = 100000;  MJArray riskFreeRates(3); MJArray forwardRates(3); contractRate = 0.006; freq = 2; TTM = 1.25;
    riskFreeRates[0] = 0.0050; riskFreeRates[1] = 0.0055; riskFreeRates[2] = 0.006; 
    forwardRates[0] = 0.055; forwardRates[1] = 0.0062; forwardRates[2] = 0.007;
    std::shared_ptr<valuationFunction> USDIRS = std::make_shared<InterestRateSwapFunction>("Interest rate swap on US 10 year yield", nominal, r, contractRate, forwardRates, freq, TTM);
    //RiskFactor 4 derivatives - Derivatives on the EUR/USD exchange rate
    nominal = 100000; double F0 = 1.15; double r_foreign = -0.003; double FXrate = (spotRates[3]); TTM = 1;
    std::shared_ptr<valuationFunction> EURUSDForward = std::make_shared<FXForwardFunction>("EUR/USD FX forward", nominal, FXrate, r, r_foreign, TTM, F0);
    double notional_d = 111000; double notional_f = 100000; FXrate = (spotRates[3]); double r_domestic = r; double r_contract_d = 0.03; double r_contract_f = 0.03; TTM = 3; freq = 1;
    std::shared_ptr<valuationFunction> EURUSDFXSwap = std::make_shared<FixedForFixedFXSwapFunction>("EUR/USD fixed for fixed FX Swap", notional_d, notional_f, FXrate, r_domestic, r_foreign, r_contract_d, r_contract_f, freq, TTM);
    //RiskFactor 5/6/7 derivatives - Derivatives on SPX,SX5E and OMX indices.
    nominal = 10000; TTM = 1.97;
    vector<vector<double>> indicesRainBowCovMatrix = myTimeSeriesHandlder.GetPartsOfCovarianceMatrix(vector<unsigned long>{4, 5, 6});
    vector<double> indices_S0_vect{ spotRates[4], spotRates[5], spotRates[6] }; 
    vector<double> indices_impvol_vect{ 0.2434175,0.215258, 0.2267145  }; vector<double> indices_div_vect{ 0.02007, 0.02313, 0.03825 };
    PayOffCallRelative SPXPayOff(3055.73); PayOffCallRelative OMXPayoff(1649.38); PayOffCallRelative SX5EPayoff(3077.92);
    vector<Wrapper<PayOff>> indicesRainbowPayoffs{ SPXPayOff, OMXPayoff, SX5EPayoff };
    std::shared_ptr<valuationFunction> indicesBestOfCallOption = std::make_shared<MonteCarloRainbowOptionFunction>("Best-Of Call Option SPY/OMX/SX5E", nominal, indices_S0_vect, r, indices_div_vect, indices_impvol_vect, indicesRainBowCovMatrix, TTM, indicesRainbowPayoffs, MCValuationNumberOfPaths, RainbowOptionType::best_of);
    std::vector<double> basketWeights{ (1 / 3.0),(1 / 3.0),(1 / 3.0) }; PayOffCall basketCallPayOff((1 / 3.0)* spotRates[4] + (1 / 3.0) * spotRates[5] + (1 / 3.0) * spotRates[6]); TTM = 3.0/12.0; nominal = 40;
    std::shared_ptr<valuationFunction> indicesBasketCallOption = std::make_shared<MonteCarloBasketOptionFunction>("Basket Call Option SPY/OMX/SX5E", nominal, indices_S0_vect, basketWeights, r, indices_div_vect, indices_impvol_vect, indicesRainBowCovMatrix, TTM, basketCallPayOff, MCValuationNumberOfPaths);
  
    //Combining the positions into new groups that will be stressed for each risk factor. Note that the same position can be stressed for any number of its risk factors
    std::shared_ptr<valuationFunction> StillFrontFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ stillFrontEuropeanCallButterflySpread1, stillFrontEuropeanCallButterflySpread2, stillFrontEuropeanCallButterflySpread3}); //Several different instruments are simulated with the process for this risk factor //stillFrontEuropeanPutMonteCarlo //stillFrontStoryTelBestOfCallOption, stillFrontStoryTelWorstOfCallOption, stillFrontStoryTelBasketCallOption ,
    std::shared_ptr<valuationFunction> StorytelFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{  storytelStock, StorytelAmericanCall}); //Note how the rainbow options are simulated for both underylings //stillFrontStoryTelBestOfCallOption, stillFrontStoryTelWorstOfCallOption, stillFrontStoryTelBasketCallOption 
    std::shared_ptr<valuationFunction> USTreasuryFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ EURUSDForward }); //Can add equitiy derivates and others here as well to simulate the risk free rate for discounting (example of stressing different risk factors for the same position)
    std::shared_ptr<valuationFunction> USDEURFXFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ EURUSDForward, EURUSDFXSwap});
    std::shared_ptr<valuationFunction> SPYFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ indicesBestOfCallOption, indicesBasketCallOption });
    std::shared_ptr<valuationFunction> OMXFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ indicesBestOfCallOption, indicesBasketCallOption });
    std::shared_ptr<valuationFunction> SX5EFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ indicesBestOfCallOption, indicesBasketCallOption });
    //Selecting the stochastic processes and risk factors to simulate for each position
    OneStepBSEngine StockSimulation(zeroDrift, StillFrontFunctions, RiskFactor::equity1);
    OneStepBSEngine StockSimulation2(zeroDrift, StorytelFunctions, RiskFactor::equity2);
    OneStepBrownianMotionEngine ShortRateSimulation(zeroDrift, USTreasuryFunctions, RiskFactor::interest_rate);
    double FXDrift = r_domestic - r_foreign;
    OneStepBSEngine FXSimulation(FXDrift, USDEURFXFunctions, RiskFactor::FX_rate);
    OneStepBSEngine SPYSimulation(zeroDrift, SPYFunctions, RiskFactor::equity1);
    OneStepBSEngine OMXSimulation(zeroDrift, OMXFunctions, RiskFactor::equity2);
    OneStepBSEngine SX5ESimulation(zeroDrift, SX5EFunctions, RiskFactor::equity3);
    //create vector of the engines that will be used in MC simulation. NOTE: Make sure to add in same order as covariance matrix was created
    vector<Wrapper<SimulationEngine>>EngineVector; 
    EngineVector.push_back(StockSimulation);
    EngineVector.push_back(StockSimulation2);
    EngineVector.push_back(ShortRateSimulation);
    EngineVector.push_back(FXSimulation);
    EngineVector.push_back(SPYSimulation);
    EngineVector.push_back(OMXSimulation);
    EngineVector.push_back(SX5ESimulation);
    try {
        //Setting up the engine and calculating the present value of the portfolio and positions
        MCEngine VAREngine(EngineVector, myCovMatrix);
        VAREngine.ValuePortfolio();
        double V0 = VAREngine.GetPortfolioValue();
        std::cout << "     Initial market values" << "\n\n";
        std::cout << "Portolio market value: " << V0 << "\n \n";
        MJArray instrumentValues = VAREngine.GetInstrumentValues();
        std::vector<std::string> instrumentIdentifiers = VAREngine.GetInstrumentIdentifiers();
        std::vector<int> instrumentNominals = VAREngine.GetInstrumentNominals();
        for (unsigned long i = 0; i < instrumentValues.size(); i++)
        {
            std::cout << instrumentIdentifiers[i] << ": \n" << "nominal: " << instrumentNominals[i] << "\n" << "Market value: " << instrumentValues[i] << "\n \n";
        }
        std::cout << "\n";
        //Creating the statistics gatherers for the simulation
        StatisticsMean meanGatherer;
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
        std::cout << "\n     Value at risk simulation \n\n";
        std::cout << "Time horizon: " << (timeHorizon * 252) << " day(s)" << "\n";
        std::cout << "Confidence Factor: " << p << "\n";
        std::cout << "Number of paths: " << NumberOfPaths << "\n";
        std::cout << "Number of paths for Monte-Carlo option valuations: " << MCValuationNumberOfPaths << "\n";
        std::cout << "Number of steps for binomial tree option valuations: " << binomialTreeSteps << "\n" << "\n";
        VAREngine.DoSimulation(gathererCombiner, instrumentGatherer, NumberOfPaths, timeHorizon);
        //Printing the resulsts from the statisticsgatheres
        vector<vector<double> > VARresults = gathererCombiner.GetResultsSoFar();
        std::cout << "\n\n     Portfolio results " << "\n";
        std::cout << "\n";
        std::cout << "forward mean: ";
        for (unsigned long j = 0; j < VARresults[0].size(); j++)
            std::cout << VARresults[0][j] << " " << "\n";
        std::cout << 100 * p << "% absolute VaR: ";
        for (unsigned long j = 0; j < VARresults[1].size(); j++)
            std::cout << VARresults[1][j] << " ";
        std::cout << "\n";
        std::cout << 100 * p << "% relative VaR: ";
        for (unsigned long j = 0; j < VARresults[2].size(); j++)
            std::cout << VARresults[2][j] * 100 << "% " << "\n";
        std::cout << 100 * p << "% relative ES: ";
        for (unsigned long j = 0; j < VARresults[3].size(); j++)
            std::cout << VARresults[3][j] * 100 << "% " << "\n";
        std::cout << "\n" << 100 * p << "% relative VaR Convergence table: " << "\n";
        for (unsigned long i = 4; i < VARresults.size(); i++) {
            for (unsigned long j = 0; j < VARresults[i].size(); j++)
            {
                std::cout << VARresults[i][j] << " ";
                if (j % 2 != 0)
                    std::cout << " paths done." << "\n";
            }
        }
        std::cout << "\n";
        std::cout << "     Instrument results " << "\n";
        std::cout << "\n";
        vector<vector<vector<double>>> InstrumentVARresults = instrumentGatherer.GetResultsSoFar();
        for (unsigned long i = 0; i < InstrumentVARresults.size(); i++)
        {
            std::cout << instrumentIdentifiers[i] << ":" << "\n";
            std::cout << "forward mean: ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][0].size(); j++)
                std::cout << InstrumentVARresults[i][0][j] << " " << "\n";
            std::cout << 100 * p << "% absolute VaR: ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][1].size(); j++)
                std::cout << InstrumentVARresults[i][1][j] << " ";
            std::cout << "\n";
            std::cout << 100 * p << "% relative VaR: ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][2].size(); j++)
                std::cout << InstrumentVARresults[i][2][j] * 100 << "% " << "\n";
            std::cout << 100 * p << "% relative ES: ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][3].size(); j++)
                std::cout << InstrumentVARresults[i][3][j] * 100 << "% " << "\n";
            std::cout << "\n";
        }

        std::cout << "\n";
        std::cout << "     Backtesting " << "\n";
        std::cout << "\n \n";
        std::cout << "Number of days to run backtest: " << daysBack << "\n";

        /// Backtesting functionality
        BackTest thisBackTest(EngineVector, myTimeSeriesHandlder);        
        thisBackTest.RunBackTest(p, timeHorizon, NumberOfPaths, daysBack, alpha);
        std::tuple<std::vector<double>, std::vector<std::vector<double>>, std::pair<unsigned long, unsigned long>> backTestResults = thisBackTest.GetResultsSoFar();
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "     Backtesting results " << "\n";
        std::cout << "\n";
        std::vector<double> results = get<0>(backTestResults);
        std::vector<std::vector<double>> detailedResults = get<1>(backTestResults);
        std::pair<unsigned long, unsigned long> binomConfInterval = get<2>(backTestResults);
        double trials = results[0]; 
        double exceedances = results[1]; 
        double ExceedancePerTrial = results[2];
        std::cout << "Number of backtested days: " << trials << " \n";
        std::cout << "Number of exceedance: " << exceedances << " \n";
        std::cout << "Exceedances / backtested days: " << ExceedancePerTrial << " \n\n";
        std::cout << "     Binomial test results \n\n";
        double lowerBound = get<0>(binomConfInterval); 
        double upperBound = get<1>(binomConfInterval);
        double singifinanceLevel = CumulativeBinomProbability(lowerBound, upperBound, (1 - p), trials);
        double pValue = BinomTestTwoSidedPValue(1-p, trials, exceedances);
        std::cout << "alpha: " << alpha << "\n"  << "lower bound: " << lowerBound << "\n" << "upper bound: " << upperBound << "\n\n";
        std::cout << "p value: " << pValue << "\n";
        if (lowerBound <= exceedances && exceedances <= upperBound)
            std::cout << "The null hypothesis that the Value at Risk model is correct can not be rejected at the " << singifinanceLevel << " confidence level.";
        std::cout << "\n \n";
        std::cout << "     Detailed backtesting results " << "\n";
        std::cout << "\n \n";
        std::cout << "Days back, Portfolio value, VaR, Realized loss, VaR Exceedance" << "\n";
        for (unsigned long i = 0; i < detailedResults.size(); i++)
        {
            for (unsigned long j = 0; j < detailedResults[i].size(); j++)
            {
                std::cout << detailedResults[i][j] << " , ";
            }
            std::cout << "\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Caught exception: " << e.what() << '\n';
    }

    //Track time and end
    auto end = sc.now();
    auto time_span = static_cast<chrono::duration<double>>(end - start);
    std::cout << "Operation took: " << time_span.count() << " seconds.";
    double tmp;
    std::cin >> tmp;

    return 0;


}