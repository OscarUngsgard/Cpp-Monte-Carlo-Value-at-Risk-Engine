//#pragma once
//#define FUNCTION_BRIDGE_H
//#include "valuationFunction.h"
//class FunctionBridge
//{
//public:
//	FunctionBridge(const FunctionBridge& original);
//	FunctionBridge(const valuationFunction& innervaluationFunction);
//	inline double valueInstrument(double S, double vol_, double r_, double T_) const;
//	~FunctionBridge();
//	FunctionBridge& operator=(const FunctionBridge& original);
//private:
//	valuationFunction* ThevaluationFunctionPtr;
//};
//inline double FunctionBridge::valueInstrument(double S, double vol_, double r_, double T_) const
//{
//	return ThevaluationFunctionPtr->valueInstrument(S, vol_, r_, T_);
//}