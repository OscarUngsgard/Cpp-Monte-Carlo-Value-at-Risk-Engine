#include "PathDependantOption.h"

PathDependantOption::PathDependantOption(const Wrapper<PathDependantPayOff>& ThePayOff_, double Expiry_) : ThePayOff(ThePayOff_), Expiry(Expiry_)
{
}

double PathDependantOption::GetExpiry() const
{
    return Expiry;
}

double PathDependantOption::OptionPayOff(std::vector<double> SpotPath) const
{
    return ThePayOff->operator()(SpotPath);
}
