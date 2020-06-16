#ifndef MJARRAYS_H
#define MJARRAYS_H
#define USE_VAL_ARRAY
#ifdef USE_VAL_ARRAY
#include <valarray>
typedef std::valarray<double> MJArray;
#else // ifdef USE_VAL_ARRAY
class MJArray
{
public:
	explicit MJArray(unsigned long size = 0);
	MJArray(const MJArray& original);
	~MJArray();
	MJArray& operator=(const MJArray& original);
	MJArray& operator=(const double& val);
	MJArray& operator+=(const MJArray& operand);
	MJArray& operator-=(const MJArray& operand);
	MJArray& operator/=(const MJArray& operand);
	MJArray& operator*=(const MJArray& operand);
	MJArray & operator+=(const double& operand);
	MJArray& operator-=(const double& operand);
	MJArray& operator/=(const double& operand);
	MJArray& operator*=(const double& operand);
	MJArray apply(double f(double)) const;
	inline double operator[](unsigned long i) const;
	inline double& operator[](unsigned long i);
	inline unsigned long size() const;
	void resize(unsigned long newSize);
	double sum() const;
	double prod() const;
	double min() const;
	double max() const;
private:
	double* ValuesPtr;
	double* EndPtr;
	unsigned long Size;
	unsigned long Capacity;
};
inline double MJArray::operator[](unsigned long i) const
{
#ifdef RANGE_CHECKING
	if (i >= Size)
	{
		throw("Index out of bounds");
	}
#endif
	return ValuesPtr[i];
}
inline double& MJArray::operator[](unsigned long i)
{
#ifdef RANGE_CHECKING
	A simple array class 277
		if (i >= Size)
		{
			throw("Index out of bounds");
		}
#endif
	return ValuesPtr[i];
}
inline unsigned long MJArray::size() const
{
	return Size;
}
#endif // ifdef USE_VAL_ARRAY
#endif // ifndef MJARRAYS_H