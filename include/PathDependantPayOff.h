#pragma once
#define PATH_DEPENDANT_PAYOFF_H
#include <vector>
class PathDependantPayOff
{
public:
	PathDependantPayOff() {};
	virtual double operator()(std::vector<double> spotPath) const = 0;
	virtual ~PathDependantPayOff() {}
	virtual PathDependantPayOff* clone() const = 0;
private:
};