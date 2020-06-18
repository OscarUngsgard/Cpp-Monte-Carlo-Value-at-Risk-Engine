#include "valuationFunction.h"
#include <iostream>
#include <algorithm>

valuationFunction::valuationFunction(std::string uniqueIdentifier_, double TTM_, int nominal_)  : f(0), TTM(TTM_), origTTM(TTM_), uniqueIdentifier(uniqueIdentifier_), nominal(nominal_)
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
	TTM = std::max(0.0, TTM - timeStep);
	return;
}

const int valuationFunction::GetNominal() const
{
	return nominal;
}

const double valuationFunction::GetOrigTTM() const
{
	return origTTM;
}

