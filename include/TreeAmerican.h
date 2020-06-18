#pragma once
#define TREE_AMERICAN_H
#include "..\include\TreeProducts.h"
#include "..\include\Wrapper.h"
#include "..\include\PayOff.h"
class TreeAmerican : public TreeProduct
{
public:
	TreeAmerican(double FinalTime, const Wrapper<PayOff>& ThePayOff_);
		virtual TreeProduct * clone() const;
	virtual double FinalPayOff(double Spot) const;
	virtual double PreFinalValue(double Spot, double Time, double DiscountedFutureValue) const;
	virtual ~TreeAmerican() {}
private:
	Wrapper<PayOff> ThePayOff;
};
