#ifndef WRAPPER_H
#define WRAPPER_H
template< class T>
class Wrapper
{
public:
	Wrapper()
	{
		DataPtr = 0;
	}
	Wrapper(const T& inner)
	{
		DataPtr = inner.clone();
	}
	Wrapper(T* DataPtr_)
	{
		DataPtr = DataPtr_;
	}
	~Wrapper()
	{
		if (DataPtr != 0)
			delete DataPtr;
	}
	Wrapper(const Wrapper<T>& original)
	{
		if (original.DataPtr != 0)
			DataPtr = original.DataPtr->clone();
		else
			DataPtr = 0;
	}
	Wrapper& operator=(const Wrapper<T>& original)
	{
	if (this != &original)
	{
	T* newPtr = (original.DataPtr != 0) ?
	original.DataPtr->clone() : 0;
	if (DataPtr != 0)
	delete DataPtr;
	DataPtr = newPtr;
	}
	return *this;
	}
		T& operator*()
	{
		return *DataPtr;
	}
	const T& operator*() const
	{
		return *DataPtr;
	}
	const T* const operator->() const
	{
		return DataPtr;
	}
	T* operator->()
	{
		return DataPtr;
	}
private:
	T* DataPtr;
};
#endif