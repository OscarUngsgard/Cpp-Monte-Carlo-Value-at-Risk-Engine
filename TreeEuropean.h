#pragma once
#define TREE_EUROPEAN_H
#include "TreeProducts.h"
#include "Wrapper.h"
#include "PayOff.h"
class TreeEuropean : public TreeProduct
{
public:
	TreeEuropean(double FinalTime, const Wrapper<PayOff>& ThePayOff_);
	virtual TreeProduct* clone() const;
	virtual double FinalPayOff(double Spot) const;
	virtual double PreFinalValue(double Spot, double Time, double DiscountedFutureValue) const;
	virtual ~TreeEuropean() {}
private:
	Wrapper<PayOff> ThePayOff;
};


