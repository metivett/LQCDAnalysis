/*
 * plot.hpp
 *
 *  Created on: Mar 5, 2013
 *      Author: Thibaut Metivet
 */

#ifndef PLOT_HPP_
#define PLOT_HPP_

#include <mgl2/mgl.h>
#include "Eigen/Dense"


namespace LQCDA {

template<typename Scalar>
mglData mgl(const std::vector<Scalar>& v)
{
	int n = v.size();
	mglData result(n);
	for(int i=0; i<n; ++i)
		result.a[i] = v[i];
	return result;
}

template<typename Scalar>
mglData mgl(const Eigen::Array<Scalar, Eigen::Dynamic, 1>& d)
{
	return mglData(d.size(), d.data());
}

}	// namespace LQCDA


#endif /* PLOT_HPP_ */
