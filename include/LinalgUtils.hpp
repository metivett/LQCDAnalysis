/*
 * LinAlgUtils.hpp
 *
 *  Created on: Dec 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef LINALGUTILS_HPP
#define LINALGUTILS_HPP

namespace LQCDA {

template<typename T, std::size_t N>
T Dot(T (&x)[N], T (&y)[N])
{
	T res = x[0]*y[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*y[n];
	}
	return res;
}
template<typename T>
T Dot(const std::vector<T>& x, const std::vector<T>& y)
{
	std::assert(x.size() == y.size());
	T res = x[0]*y[0];
	for(int n=1; n<x.size(); n++)
	{
		res += x[n]*y[n];
	}
	return res;
}
inline double Dot(double x, double y) { return x*y; }
inline float Dot(float x, float y) { return x*y; }

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
template<typename T>
T DotSquare(const std::vector<T>& x)
{
	T res = x[0]*x[0];
	for(int n=1; n<x.size(); n++)
	{
		res += x[n]*x[n];
	}
	return res;
}
inline double DotSquare(double x) { return x*x; }
inline inline float DotSquare(float x) { return x*x; }

template<typename T, std::size_t N>
T WeightedDot(T (&x)[N], T (&y)[N], T (&w)[N])
{
	T res = x[0]*y[0]*w[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*y[n]*w[n];
	}
	return res;
}
template<typename T>
T WeightedDot(const std::vector<T>& x, const std::vector<T>& y, const std::vector<T>& w)
{
	std::assert(x.size() == y.size() && x.size() == w.size());
	T res = x[0]*y[0]*w[0];
	for(int n=1; n<x.size(); n++)
	{
		res += x[n]*y[n]*w[n];
	}
	return res;
}
inline double WeightedDot(double x, double y, double w) { return x*y*w; }
inline float WeightedDot(float x, float y, float w) { return x*y*w; }

template<typename T, std::size_t N>
T WeightedDotSquare(T (&x)[N], T (&w)[N])
{
	T res = x[0]*x[0]*w[0];
	for(int n=1; n<N; n++)
	{
		res += x[n]*x[n]*w[n];
	}
	return res;
}
template<typename T>
T WeightedDotSquare(const std::vector<T>& x, const std::vector<T>& w)
{
	std::assert(x.size() == w.size());
	T res = x[0]*x[0]*w[0];
	for(int n=1; n<x.size(); n++)
	{
		res += x[n]*x[n]*w[n];
	}
	return res;
}
inline double WeightedDotSquare(double x, double w) { return x*x*w; }
inline float WeightedDotSquare(float x, float w) { return x*x*w; }

template<typename T>
std::vector<T> Inverse(const std::vector<T>& x)
{
	std::vector<T> res(x.size());
	for(int n=1; n<x.size(); n++)
	{
		std::assert(x[n] != 0.);
		res[n] = 1./x[n];
	}
	return res;
}
inline double Inverse(double x) { std::assert(x != 0.); return 1./x; }
inline float Inverse(float x) { std::assert(x != 0.); return 1./x; }

}

#endif