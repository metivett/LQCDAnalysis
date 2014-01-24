/*
 * LinAlgUtils.hpp
 *
 *  Created on: Dec 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef LINALGUTILS_HPP
#define LINALGUTILS_HPP

 #include "Vector.hpp"

namespace LQCDA {

template<typename T, unsigned int N>
T Dot(T (&x)[N], T (&y)[N])
{
	T res = x[0]*y[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*y[n];
	}
	return res;
}
template<typename T, unsigned int N>
T Dot(const Vector<T, N>& x, const Vector<T, N>& y)
{
	T res = x[0]*y[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*y[n];
	}
	return res;
}

template<typename T, std::size_t N>
T DotSquare(T (&x)[N])
{
	T res = x[0]*x[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*x[n];
	}
	return res;
}
template<typename T, unsigned int N>
T DotSquare(const Vector<T, N>& x)
{
	T res = x[0]*x[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*x[n];
	}
	return res;
}

template<typename T, unsigned int N>
T WeightedDot(T (&x)[N], T (&y)[N], T (&w)[N])
{
	T res = x[0]*y[0]*w[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*y[n]*w[n];
	}
	return res;
}
template<typename T, unsigned int N>
T WeightedDot(const Vector<T, N>& x, const Vector<T, N>& y, const Vector<T, N>& w)
{
	T res = x[0]*y[0]*w[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*y[n]*w[n];
	}
	return res;
}

template<typename T, unsigned int N>
T WeightedDotSquare(T (&x)[N], T (&w)[N])
{
	T res = x[0]*x[0]*w[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*x[n]*w[n];
	}
	return res;
}
template<typename T, unsigned int N>
T WeightedDotSquare(const Vector<T, N>& x, const Vector<T, N>& w)
{
	T res = x[0]*x[0]*w[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*x[n]*w[n];
	}
	return res;
}

template<typename T, unsigned int N>
Vector<T, N> Inverse(const Vector<T, N>& x)
{
	Vector<T, N> res;
	for(int n=0; n<N; n++)
	{
		assert(x[n] != 0.);
		res[n] = 1./x[n];
	}
	return res;
}

}

#endif