#pragma warning( disable : 4786 )
#pragma once
#include "TreeProducts.h"
#include <vector>
#include "Arrays.h"
class SimpleBinomialTree
{
public:
	SimpleBinomialTree(double Spot_, const double r_, const double d_, double Volatility_, unsigned long Steps, double Time);
	double GetThePrice(const TreeProduct& TheProduct);
protected:
	void BuildTree();
private:
	double Spot;
	double r;
	double d;
	double Volatility;
	unsigned long Steps;
	double Time;
	bool TreeBuilt;
	std::vector<std::vector<std::pair<double, double> > > TheTree;
	MJArray Discounts;
	double p; //Risk neutral probability of an up-move
};