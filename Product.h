#pragma once
#define PRODUCT_H
#include "Arrays.h"
#include <vector>
#include "Cashflow.h"

class PathDependent
{
public:
	PathDependent(const MJArray& LookAtTimes_);
	const MJArray& GetLookAtTimes() const;
	virtual unsigned long CashFlows(const MJArray& SpotValues,
		std::vector<CashFlow>&
		GeneratedFlows) const = 0;
	virtual PathDependent* clone() const = 0;
	virtual ~PathDependent() {}
private:
	MJArray LookAtTimes;
};