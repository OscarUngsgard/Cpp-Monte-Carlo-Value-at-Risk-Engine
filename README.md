# C++ Monte Carlo Value at Risk Engine

## Background and overview

This program is the culmination of two months of work to create a fully functional and comprehensive Monte Carlo Value at Risk engine for calculating the risk of a financial portfolio, on a total portfolio level as well as on an individual instrument level. The program was written in my spare time immediately following graduating with a Master’s degree in Financial Mathematics while at the same time working full time as a quantitative analyst at an investment bank. The task was undertaken with the motivation of solidifying my knowledge of the fundamentals of risk and valuations calculations and teaching myself proper usage of C++ as a tool for implementing sound financial engineering solutions with object-oriented programming.

The knowledge base drawn upon when implementing the financial models in C++ primarily comes from the book *C++ Design Patterns and Derivatives Pricing* by Mark Joshi. The program as such readily makes use of design patterns to produce reusable and extensible code following the open/closed principle of object-oriented programming. This has resulted in a code base that can be seamlessly extended in a logical way to handle additional functionality such as support for additional instrument types and calculations of different statistics (VaR itself being one statistic), one could even straightforwardly extend the program by adding additional stochastic processes for the path simulations of the risk factors such as a jump diffusion processes or allowing for stochastic volatility.

## Structure and components of the program

Attached in the repository is a class diagram of the program showing the structure of the system. The system comprises the core *MCEngine* class where the Monte Carlo simulation takes place along with 3 key base classes that together make up the structure of the program. These are the pure virtual classes: *SimulationEngine*, *ValuationFunction* and *MCStatistics*. 

An object of the class *ValuationFunction* serves the purpose of representing one position in the portfolio, it has members such as *nominal* and *uniqueIdentifier* that describe the position with instrument specific attributes such as the spot value of an underlying risk factor or the implied volatility of an instrument being members of the relevant derived classes. Examples of such derived classes are various equity derivatives, Bonds and IRS, and FX forwards. The primary function of *ValuationFunction* is to value the position which is done in the function *ValueInstrument* at each step in the Monte Carlo simulation. Note that the *ValueInstrument* function itself may employ heavy numerical procedures when necessary, such as constructing a binomial tree for American options or a Monte Carlo simulation of its own on a position level to value derivatives like rainbow options where no other approach is suitable.

The *SimulationEngine* class takes care of the path simulation for each of the underlying risk factors that affect the portfolio and takes as input in its constructor an object of type *ValuationFunction* (the custom made pointer class *Wrapper* facilitates inputs of virtual objects and is frequently employed throughout the code for this purpose). One derived class of *ValuationFunction* is *FunctionCombiner* which stores several inner *ValuationFunctions* and passes along in each of its functions the task to the inner functions, as such an object of type *SimulationEngine* can be told to simulate one risk factor in several instruments on the same underlying. The *ValuationFunctions* are passed into the *SimulationEngine* using the smart pointer *shared_ptr* and care is taken to only value each instrument once for each path (regardless of how many times it’s submitted to the engine) and as such it’s possible to also simulate several underlying risk factors for one instrument. An example would be to stress both the exchange rate and the base currency discounting rate when simulating the value of an FX forward. The primary function of the *SimulationEngine* is to simulate a path of the underlying risk factor which is done in the function *DoOnePath*. This is a pure virtual function that is implemented in each of the derived classes according to the type of stochastic process simulated, currently there is functionality for simulating a geometric Brownian motion in the derived class *OneStepBSEngine*, suitable for vanilla equity derivatives and FX rates, as well as simulating a simple Brownian motion in *OneStepBrownianMotionEngine*, suitable for interest rates. Functionality for additional stochastic processes as described previously can be achieved by simply implementing the virtual functions in a new derived *SimulationEngine* class. Care is also taken in the function *DoOnePath* to handle the edge cases where the VaR horizon is greater than the time to maturity of an instrument, at which point it will only simulate a path up to the maturity of that instrument instead of the entire VaR horizon.

The *MCStatistics* class is delegated the task of aggregating the results of the Monte Carlo simulation and processing it in order to produce meaningful statistics, such as the Value at Risk, from the simulation. Because of its generality the *MCStatistics* is also used to aggregate results from Monte Carlo option valuations within the Value at Risk simulation. Much like we have a *FunctionCombiner* class derived from *ValuationFunction* we have a derived from *MCStatistics* class *StatisticsCompiler*, and just as the *FunctionCombiner* allows the simulation of several instruments for one risk factor *StatisticsCompiler* allows the usage of several derived *MCStatistics* for the same Monte Carlo portfolio simulation so that you may produce several statistics as needed. Some examples of statistics produced by *MCStatistics* derived classes are: relative VaR, relative Expected Shortfall, forward mean value, and a convergence table which takes as input any *MCStatistics* and calculates the statistic at each power of two number of paths done. All of these can be freely combined using the *StatisticsCompiler*. The *MCStatistics* class saves the statistics in a two dimensional vector of doubles which can be retrieved and outputted after simulation. An additional related, but not derived from *MCStatistics*, class that is used in the simulation is *InstrumentStatisticsGatherer* which as the name implies does very much the same thing as the *MCStatistics* classes but on an individual position basis. Much like the *StatisticsCompiler* the *InstrumentStatisticsGatherer* takes as input in its constructor a vector of *MCStatistics* objects.

On top of the classes derived from these three base classes there is an ensemble of helper classes to facilitate their usage along with a few more general classes and functions for numerical computations or tasks such as producing a covariance matrix from time series and creating correlated normal variates for the simulation using a Cholesky decomposition, as in the function *GetArraysOfCorrelatedGauassiansByBoxMuller* found in the file *Random*.

The core class that instructs the sequencing of the Monte Carlo Value at Risk simulation is, as previously mentioned, the class *MCEngine* which takes as input in its constructor: a vector of *SimulationEngine* objects (each having as a member an object of type *ValuationFunction*) here called *EngineVector*, and the covariance matrix of the risk factors going into the simulation. It is when calling the *DoSimulation* function of the MCEngine that the magic happens. The *DoSimulation* function takes as arguments an object of type *MCStatistics*, an object of type *InstrumentStatisticsGatherer*, as well as the time horizon and the number of iterations that will be carried out in the simulation. By and large *DoSimulation* works by for each iteration carrying out the following tasks:
1. Simulating paths for the underlying risk factors using the different *SimulationEngine* objects in *EngineVector*.
2. Valuing the instruments and portfolio using the functionality provided in the *ValuationFunction* objects.
3. Saving the results in the two statistics gathering objects.

Care is taken to make this process as optimized as possible but the computational bottleneck will in most cases be in 2. as a full re-valuation of the instruments needs to be completed for each iteration which for instrument types with complicated payoffs that need to be numerically calculated quickly can become very computationally expensive.

## Setting up the simulation in the main function

An example of a risk calculation being done for a portfolio is given in the main function of the program. In order to run the simulation we first need to calculate a covariance matrix of the risk factors and find the current spot rates for the underlyings. These preliminary tasks are handled by the class *TimeSeriesHandler* which transforms a two dimensional vector of closing prices to returns and then calculates the sample covariance matrix. Additional functions could be added to this class to support more refined methods such as exponentially weighted moving averages for the calculation of the covariance matrix. The class *CSVReader* allows the input of a headerless CSV file of closing prices for a series of risk factors from which the covariance matrix can be calculated. A short python script can be found in the repository which extracts these values from Yahoo Finance and saves them to such a CSV file. The covariance matrix and spot rates can of course also manually be inputted.

After this is done the positions are created as *Shared_ptr* objects pointing to derived ValuationFunction classes, several examples are shwon in the example main function. The positions are then grouped into *FunctionCombiner* objects in the way that they will be simulated in the *SimulationEngine* objects which are then also created.
At this point we may wish to value the initial portfolio and instruments and output the result, which is done in the example. Continuing on the *MCStatistics* objects are created as needed (and compiled together in a *StatisticsCompiler* object) along with a *InstrumentStatisticsGatherer* object. At this point we have all the ingredients needed to run our simulation and we do so by calling DoSimulation with our created covariance matrix, StatisticsCompiler, desired number of paths, and VaR horizon. The results are carefully outputted from the *StatisticsCompiler* and *InstrumentStatisticsGatherer* to show the simulation results.
