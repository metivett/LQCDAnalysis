/*
 * Sample.hpp
 *
 *  Created on: Apr 08, 2014
 *      Author: Thibaut Metivet
 */

#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include "StatSample.hpp"

BEGIN_NAMESPACE(LQCDA)

template<typename T>
class Sample;

BEGIN_NAMESPACE(internal)

template<typename T> 
struct sample_traits<Sample<T>>
{
	typedef T SampleElement;
};

END_NAMESPACE // internal

template<typename T>
class Sample
: public StatSample<Sample<T>>
{
protected:
	// Typedefs
	typedef StatSample<Sample<T>> Base;

public:
	// Constructors
	Sample()
	: Base()
	{}
	explicit Sample(unsigned int size)
	: Base(size)
	{}

	// Destructor
	virtual ~Sample() = default;
};

#define FOR_SAMPLE(sample, s) \
for(unsigned int s = 0; s < (sample).size(); ++s)


/******************************************************************************
*                     Specialization for reference types                     *
******************************************************************************/

template<typename T>
class Sample<T&>
: public Eigen::Map<Array<T, Dynamic, 1>, 0, Eigen::InnerStride<>>
{
protected:
	// Typedefs
	typedef Eigen::Map<Array<T, Dynamic, 1>, 0, Eigen::InnerStride<>> Base;
	typedef T& nested_type;

public:
	// Constructors
	explicit Sample(T* dataPtr, unsigned int size, unsigned int stride =1);
	//EIGEN_EXPR_CTOR(Sample, Sample<T&>, Base, ArrayExpr)

	// Destructor
	virtual ~Sample() = default;

	// Accessors
	unsigned int size() const;
	void resize(unsigned int size);

	// Operators
	T& operator[](unsigned int s);
	const T& operator[](unsigned int s) const;

	// Statistics
	T mean(unsigned int begin = 0, int n = -1) const;
	T covariance(const Sample<T>& sample, unsigned int begin = 0, int n = -1) const;
	auto covarianceMatrix(const Sample<T>& sample, unsigned int begin, int n) const
		-> decltype(REDUX::tensorProd(
			std::declval<Sample<T>>().segment(begin, n).redux(&REDUX::sum<T>),
			sample.segment(begin, n).redux(&REDUX::sum<T>)));
	T variance(unsigned int begin = 0, int n = -1) const;
	auto varianceMatrix(unsigned int begin = 0, int n = -1) const
		-> decltype(std::declval<Sample<T>>().covarianceMatrix(std::declval<Sample<T>>(), begin, n));
};

template<typename T>
Sample<T&>::Sample(T* dataPtr, unsigned int size, unsigned int stride)
: Base(dataPtr, size, Eigen::InnerStride<>(stride))
{}

template<typename T>
unsigned int Sample<T&>::size() const
{
	return Base::size();
}

template<typename T>
void Sample<T&>::resize(unsigned int size)
{
	Base::resize(size);
}

template<typename T>
T& Sample<T&>::operator[](unsigned int s)
{
	return Base::operator[](s);
}

template<typename T>
const T& Sample<T&>::operator[](unsigned int s) const 
{
	return Base::operator[](s);
}

template<typename T>
T Sample<T&>::mean(unsigned int begin, int n) const
{
	T result;
	const unsigned int len = (n >= 0)? n: size();
	if(len)
	{
		result = this->segment(begin, len).redux(&REDUX::sum<T>);
	}
	return result / static_cast<double>(len);
}

template<typename T>
T Sample<T&>::covariance(const Sample<T>& sample, unsigned int begin, int n) const
{
	const unsigned int len = (n >= 0)? n: size();
	if(len)
	{
		auto sampleV = sample.segment(begin, len);
		auto thisV = this->segment(begin, len);

		// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
		return ( thisV.binaryExpr(sampleV, &REDUX::prod<T>).redux(&REDUX::sum<T>) 
			- REDUX::prod(thisV.redux(&REDUX::sum<T>), sampleV.redux(&REDUX::sum<T>)) / static_cast<double>(len) )
			/ static_cast<double>(len - 1);
	}
}

template<typename T>
auto Sample<T&>::covarianceMatrix(const Sample<T>& sample, unsigned int begin, int n) const
-> decltype(REDUX::tensorProd(std::declval<Sample<T>>().segment(begin, n).redux(&REDUX::sum<T>), sample.segment(begin, n).redux(&REDUX::sum<T>)))
{
	const unsigned int len = (n >= 0)? n: size();
	if(len)
	{
		auto sampleV = sample.segment(begin, len);
		auto thisV = this->segment(begin, len);

		// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
		return ( thisV.binaryExpr(sampleV, &REDUX::tensorProd<T>).redux(&REDUX::sum<T>) 
			- REDUX::tensorProd(thisV.redux(&REDUX::sum<T>), sampleV.redux(&REDUX::sum<T>)) / static_cast<double>(len) )
			/ static_cast<double>(len - 1);
	}
}

template<typename T>
T Sample<T&>::variance(unsigned int begin, int n) const
{
	return covariance(*this, begin, n);
}

template<typename T>
auto Sample<T&>::varianceMatrix(unsigned int begin, int n) const
-> decltype(std::declval<Sample<T>>().covarianceMatrix(std::declval<Sample<T>>(), begin, n))
{
	return covarianceMatrix(*this, begin, n);
}

END_NAMESPACE // LQCDA

#endif // SAMPLE_HPP