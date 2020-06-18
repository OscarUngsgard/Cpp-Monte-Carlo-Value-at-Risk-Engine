//System functions
#include <iostream>
#include <cmath>
#include <chrono>
#include <memory>
//General classes and functions
#include "..\include\ReadCSV.h"
#include "..\include\Wrapper.h"
//Numerical calculations
#include "..\include\TimeSeriesHandler.h"
#include "..\include\Random.h"
#include "..\include\Cholezky.h"
//enums
#include "..\include\RiskFactors.h"
#include "..\include\AbsOrRelReturns.h"
#include "..\include\RainbowOptionTypes.h"
//Statistics calculations e.g. VaR from quantile
#include "..\include\MCStatistics.h"
#include "..\include\StatisticsMean.h"
#include "..\include\StatisticsRelVaR.h"
#include "..\include\StatisticsAbsVaR.h"
#include "..\include\StatisticsRelES.h"
#include "..\include\StatisticsQuantiles.h"
#include "..\include\ConvergenceTable.h"
#include "..\include\StatisticsCompiler.h"
#include "..\include\InstrumentStatisticsGatherer.h"
// Main monte carlo VaR simulations engine
#include "..\include\MCEngine.h"
//Inner VaR simulation Engines. E.g. Geometric brownian motion for stocks and simple brownian motion for short rate
#include "..\include\SimulationEngine.h"
#include "..\include\OneStepBSEngine.h"
#include "..\include\OneStepBrownianMotion.h"
//valuation functions used as input into the simulationengine
#include "..\include\valuationFunction.h"
#include "..\include\FunctionCombiner.h"
#include "..\include\EquityForwardFunction.h"
#include "..\include\FXForwardFunction.h"
#include "..\include\BSCallFunction.h"
#include "..\include\BSPutFunction.h"
#include "..\include\AmericanGeneralPayOffFunction.h" // Note: Risk for American options are computed with a full revaluation using a binomial treeand hence will increase the computation time significantly if included in the portfolio
#include "..\include\AmericanCallFunction.h" 
#include "..\include\AmericanPutFunction.h"
#include "..\include\StockPriceFunction.h"
#include "..\include\BondFunction.h"
#include "..\include\FRAFunction.h" 
#include "..\include\InterestRateSwapFunction.h"
#include "..\include\FixedForFixedFXSwapFunction.h"
//monte carlo option pricers
#include "..\include\MonteCarloVanillaOptionFunction.h" //can be freely used to simulate any payoff 
#include "..\include\MonteCarloRainbowOptionFunction.h" //Note: Monte Carlo valuations increase computation time significantly but are the only sensible alternative for rainbow options
#include "..\include\MonteCarloBasketOptionFunction.h"
//Payoffs
#include "..\include\PayOffCall.h"
#include "..\include\PayOffPut.h"

using namespace std;

int main()
{
    //Start timer and set seed
    chrono::steady_clock sc;   
    auto start = sc.now();
    srand(2);

    //Read in file of prices to get most recent prices and covariance matrix
    vector<vector<double>> myTimeSeries = parse2DCsvFile("riskFactorsnew.csv");
    vector<AbsOrRel> AbsOrRelReturns{ AbsOrRel::rel, AbsOrRel::rel, AbsOrRel::abs, AbsOrRel::rel }; //Specifiy if the relative returns or absolute returns are normally distributed for the time series 
    TimeSeriesHandler myTimeSeriesHandlder(myTimeSeries, AbsOrRelReturns);
    myTimeSeriesHandlder.createCovarianceMatrix();
    vector<vector<double>> myCovMatrix = myTimeSeriesHandlder.GetCovarianceMatrix();
    vector<double> spotRates = myTimeSeriesHandlder.GetMostRecentValues();
  
    ///output the covariance matrix and spot rates used  
    //vector<vector<double>>  myReturns = myTimeSeriesHandlder.GetReturns();
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

    ///Functionality to manually create Covariance matrix and set current prices///
    //vector<double> spotRates{ 690, 196.4, 0.00648, 1.1079 };
    //std::vector<double> covmatrow1{ 0.24011, 0.0699455, 0.000565568, -7.56869e-05 };
    //std::vector<double> covmatrow2{ 0.0699455, 0.241647, 0.000646747, -0.000195283 };
    //std::vector<double> covmatrow3{ 0.000565568, 0.000646747, 0.000103739, -5.52115e-05 };
    //std::vector<double> covmatrow4{ -7.56869e-05, -0.000195283, -5.52115e-05, 0.00492995 };
    //std::vector<std::vector<double>> myCovMatrix{ covmatrow1, covmatrow2, covmatrow3, covmatrow4 };

    //Declare variables and set some constants
    const unsigned long binomialTreeSteps = 10; //for american options
    const unsigned long NumberOfPaths = pow(10, 4);
    const unsigned long MCValuationNumberOfPaths = 5*pow(10, 3); //For monte-carlo option valuations of e.g. for Best-of/worst-of options
    double timeHorizon = 20.0 / 252.0;
    double S0;    double TTM;    double Strike;    double d;    double contractRate;    double facevalue;
    double yield;    signed long nominal;    unsigned long couponFreq;    unsigned long freq;    double couponRate;
    double zeroDrift = 0;    double r = 0.0035;

    //Creating the positions//
    //RiskFactor 1 derivatives - A straddle
    nominal = 100; d = 0; S0 = spotRates[0]; Strike = spotRates[0]; double stilfrontImpvol = 0.5; TTM =  3.0 / 12.0;
    std::shared_ptr<valuationFunction> stillFrontEuropeanCall = std::make_shared<BSCallFunction>("StillFront 3 month ATM European call", nominal, S0, r, d, stilfrontImpvol, TTM, Strike);
    std::shared_ptr<valuationFunction> stillFrontEuropeanPut = std::make_shared<BSPutFunction>("StillFront 3 month ATM European put", nominal, S0, r, d, stilfrontImpvol, TTM, Strike);
    //RiskFactor 2 derivatives - A covered calll
    nominal = 140; S0 = spotRates[1];
    std::shared_ptr<valuationFunction> storytelStock = std::make_shared<StockPriceFunction>("Storytel", nominal, S0);
    nominal = -200; double storytelDivYield = 0.03; Strike = 0.9 * spotRates[1]; double storytelImpvol = 0.5; TTM = 3.0 / 12.0;
    PayOffCall StorytelITMCallPayOff(0.9 * spotRates[1]);
    std::shared_ptr<valuationFunction> StorytelAmericanCall = std::make_shared<AmericanGeneralPayOffFunction>("Storytel 3 month In-the-money American call", nominal, S0, r, storytelDivYield, storytelImpvol, TTM, StorytelITMCallPayOff, binomialTreeSteps);
    //RiskFactor 2 and 3 Derivatives - Some sexy exotics
    nominal = 100;
    vector<vector<double>> rainBowCovMatrix = myTimeSeriesHandlder.GetPartsOfCovarianceMatrix(vector<unsigned long>{0, 1});
    vector<double> S0_vect{ spotRates[0], spotRates[1] }; vector<double> impvol_vect{ stilfrontImpvol, storytelImpvol }; vector<double> div_vect{ 0, storytelDivYield };
    PayOffCall StillfrontATMCallPayOff(S0_vect[0]); PayOffCall storytelATMCallPayOff(S0_vect[1]);
    vector<Wrapper<PayOff>> rainbowPayoffs{ StillfrontATMCallPayOff, storytelATMCallPayOff };
    std::shared_ptr<valuationFunction> stillFrontStoryTelWorstOfCallOption = std::make_shared<MonteCarloRainbowOptionFunction>("Worst-Of Call Option StilFront/StoryTel", nominal, S0_vect, r, div_vect, impvol_vect, rainBowCovMatrix, TTM, rainbowPayoffs, MCValuationNumberOfPaths, RainbowOptionType::worst_of);
    nominal = 100;
    std::shared_ptr<valuationFunction> stillFrontStoryTelBestOfCallOption = std::make_shared<MonteCarloRainbowOptionFunction>("Best-Of Call Option StilFront/StoryTel", nominal, S0_vect, r, div_vect, impvol_vect, rainBowCovMatrix, TTM, rainbowPayoffs, MCValuationNumberOfPaths, RainbowOptionType::best_of);
    PayOffCall basketCallPayOff(S0_vect[0] * 0.5 + S0_vect[1] * 0.5); std::vector<double> basketWeights{ 0.4,0.6 };
    std::shared_ptr<valuationFunction> stillFrontStoryTelBasketCallOption = std::make_shared<MonteCarloBasketOptionFunction>("Basket Call Option StilFront/StoryTel", nominal, S0_vect, basketWeights, r, div_vect, impvol_vect, rainBowCovMatrix, TTM, basketCallPayOff, MCValuationNumberOfPaths);
    //RiskFactor 3 derivatives - Derivatives on the US 10 year treasury yiel
    nominal = 100; yield = spotRates[2];  facevalue = 100; couponFreq = 2; couponRate = 0.01; TTM = 10;
    std::shared_ptr<valuationFunction> USTreasuryBond = std::make_shared<BondFunction>("10 year US treasury bond", nominal, yield, facevalue, couponRate, couponFreq, TTM);
    nominal = 100000;  MJArray riskFreeRates(3); MJArray forwardRates(3); contractRate = 0.006; freq = 2; TTM = 1.25;
    riskFreeRates[0] = 0.0050; riskFreeRates[1] = 0.0055; riskFreeRates[2] = 0.006; 
    forwardRates[0] = 0.055; forwardRates[1] = 0.0062; forwardRates[2] = 0.007;
    std::shared_ptr<valuationFunction> USDIRS = std::make_shared<InterestRateSwapFunction>("Interest rate swap on US 10 year yield", nominal, r, contractRate, forwardRates, freq, TTM);
    //RiskFactor 4 derivatives - Derivatives on the EUR/USD exchange rate
    nominal = 100000; double F0 = 1.15; double r_foreign = -0.003; double FXrate = (spotRates[3]); TTM = 1;
    std::shared_ptr<valuationFunction> EURUSDForward = std::make_shared<FXForwardFunction>("EUR/USD FX forward", nominal, FXrate, r, r_foreign, TTM, F0);
    double notional_d = 1110000; double notional_f = 1000000; FXrate = (spotRates[3]); double r_domestic = r; double r_contract_d = 0.03; double r_contract_f = 0.03; TTM = 3; freq = 1;
    std::shared_ptr<valuationFunction> EURUSDFXSwap = std::make_shared<FixedForFixedFXSwapFunction>("EUR/USD fixed for fixed FX Swap", notional_d, notional_f, FXrate, r_domestic, r_foreign, r_contract_d, r_contract_f, freq, TTM);
    
    //Combining the positions into new groups that will be stressed for each risk factor. Note that the same position can be stressed for any number of its risk factors
    std::shared_ptr<valuationFunction> StillFrontFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{stillFrontEuropeanCall, stillFrontEuropeanPut, stillFrontStoryTelBestOfCallOption, stillFrontStoryTelWorstOfCallOption, stillFrontStoryTelBasketCallOption }); //Several different instruments are simulated with the process for this risk factor 
    std::shared_ptr<valuationFunction> StorytelFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ storytelStock, StorytelAmericanCall, stillFrontStoryTelBestOfCallOption, stillFrontStoryTelWorstOfCallOption, stillFrontStoryTelBasketCallOption }); //Note how the rainbow options are simulated for both underylings
    std::shared_ptr<valuationFunction> USTreasuryFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ USTreasuryBond, USDIRS, EURUSDForward,  StorytelAmericanCall }); //Can add equitiy derivates and others here as well to simulate the risk free rate for discounting (example of stressing different risk factors for the same position)
    std::shared_ptr<valuationFunction> USDEURFXFunctions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ EURUSDForward });
    
    //Selecting the stochastic processes and risk factors to simulate for each position
    OneStepBSEngine StockSimulation(zeroDrift, StillFrontFunctions, RiskFactor::equity1);
    OneStepBSEngine StockSimulation2(zeroDrift, StorytelFunctions, RiskFactor::equity2);
    OneStepBrownianMotionEngine ShortRateSimulation(zeroDrift, USTreasuryFunctions, RiskFactor::interest_rate);
    double FXDrift = r_domestic - r_foreign;
    OneStepBSEngine FXSimulation(FXDrift, USDEURFXFunctions, RiskFactor::FX_rate);
    //create vector of the engines that will be used in MC simulation. NOTE: Make sure to add in same order as covariance matrix was created
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
        double p = 0.95;
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
        std::cout <<"\n     Value at risk simulation \n\n";
        std::cout << "Number of paths: " << NumberOfPaths << "\n";
        std::cout << "Time horizon: " << (timeHorizon * 252) << " day(s)" << "\n";
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
                std::cout << InstrumentVARresults[i][1][j]<< " ";
            std::cout << "\n";
            std::cout << 100 * p << "% relative VaR: ";
            for (unsigned long j = 0; j < InstrumentVARresults[i][2].size(); j++)
                std::cout << InstrumentVARresults[i][2][j]*100 << "% " << "\n";
            std::cout << 100 * p << "% relative ES: ";
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
    auto end = sc.now();
    auto time_span = static_cast<chrono::duration<double>>(end - start);
    std::cout << "Operation took: " << time_span.count() << " seconds.";
    double tmp;
    std::cin >> tmp;
    return 0;
}