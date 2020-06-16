#include "BinomialTree.h"

SimpleBinomialTree::SimpleBinomialTree(double Spot_, const double r_, const double d_, double Volatility_, unsigned long Steps_, double Time_) : Spot(Spot_), r(r_), d(d_), Volatility(Volatility_), Steps(Steps_), Time(Time_), Discounts(Steps_), p(0.5)
{
	TreeBuilt = false;
}

double SimpleBinomialTree::GetThePrice(const TreeProduct&
	TheProduct)
{
	if (!TreeBuilt)
		BuildTree();
	if (TheProduct.GetFinalTime() != Time)
		throw("mismatched product in SimpleBinomialTree");
	//Caclculate risk neutral probabiliy of up-move, see e.g. Hull
	double a = exp((r - d) * (Time / Steps));
	double up = exp(Volatility * sqrt(Time / Steps));
	double down = 1 / up;
	p = (a - down) / (up - down);
	for (long j = -static_cast<long>(Steps), k = 0; j <= static_cast<long>(Steps); j = j + 2, k++)
	{
		TheTree[Steps][k].second = TheProduct.FinalPayOff(TheTree[Steps][k].first);
	}
	for (unsigned long i = 1; i <= Steps; i++)
	{
		unsigned long index = Steps - i;
		double ThisTime = index * Time / Steps;
		for (long j = -static_cast<long>(index), k = 0; j <= static_cast<long>(index); j = j + 2, k++)
		{
			double Spot = TheTree[index][k].first;
			double futureDiscountedValue = Discounts[index] * (TheTree[index + 1][k].second * (1-p) + (p) * TheTree[index + 1][k + 1].second);
			TheTree[index][k].second = TheProduct.PreFinalValue(Spot, ThisTime,futureDiscountedValue);
		}
	}
	return TheTree[0][0].second;
}

void SimpleBinomialTree::BuildTree()
{
	TreeBuilt = true;
	TheTree.resize(Steps + 1);
	for (unsigned long i = 0; i <= Steps; i++)
	{
		TheTree[i].resize(i + 1);
		double sd = Volatility * sqrt(Time / Steps);
		for (long j = -static_cast<long>(i), k = 0; j <= static_cast<long>(i); j = j + 2, k++)
			TheTree[i][k].first = Spot * exp(j * sd);
	}
	for (unsigned long l = 0; l < Steps; l++)
	{
		Discounts[l] = exp(-r * (Time / Steps));
	}
}
