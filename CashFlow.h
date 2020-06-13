#pragma once
class CashFlow
{
public:
	double Amount;
	double TimeIndex;
	CashFlow(double TimeIndex_ = 0.0, double Amount_ = 0.0)
		: TimeIndex(TimeIndex_),
		Amount(Amount_) {};
};