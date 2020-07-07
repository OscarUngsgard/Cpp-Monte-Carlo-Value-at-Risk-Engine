#pragma once
#define PATH_DEPENDANT_OPTION_H
#include "Wrapper.h"
#include <vector>
#include "PathDependantPayOff.h"
class PathDependantOption
{
public:
	PathDependantOption(const Wrapper<PathDependantPayOff>& ThePayOff_, double Expiry_);
	double GetExpiry() const;
	double OptionPayOff(std::vector<double> SpotPath) const;
private:
	double Expiry;
	Wrapper<PathDependantPayOff> ThePayOff;
};