/*
 * Chi2.hpp
 *
 *  Created on: Dec 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef CHI2_HPP
#define CHI2_HPP

#include <vector>
#include "LinalgUtils.hpp"

namespace LQCDA {

template<typename T>
double Chi2(const std::vector<T>& v)
{
	double res = 0.;

	for(int i=0; i<v.size(); i++)
	{
		res += DotSquare(v[i]);
	}

	return res;
}

template<typename T>
double Chi2(const std::vector<T>& v, const std::vector<T>& e)
{
	double res = 0.;

	for(int i=0; i<v.size(); i++)
	{
		res += WeightedDotSquare(v[i], Inverse(e[i]));
	}

	return res;
}

 }

#endif
