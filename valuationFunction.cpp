#include "valuationFunction.h"
#include <minmax.h>

valuationFunction::valuationFunction(std::string uniqueIdentifier_, double TTM_)  : f(0), TTM(TTM_), uniqueIdentifier(uniqueIdentifier_)
{
}

double valuationFunction::GetValue() const
{
	return f;
}

std::vector<std::string> valuationFunction::GetuniqueIdentifier() const
{
	std::vector<std::string> stringVector(1);
	stringVector[0] = uniqueIdentifier;
	return stringVector;
}

void valuationFunction::UpdateTTM(double timeStep)
{
	TTM = max(0, TTM - timeStep);
	return;
}
