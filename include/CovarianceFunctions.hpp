/*
 * CovarianceFunctions.hpp
 *
 *  Created on: Jun 30, 2013
 *      Author: Thibaut Metivet
 */

#ifndef COVARIANCE_FUNCTIONS_HPP_
#define COVARIANCE_FUNCTIONS_HPP_

#include "Eigen/Dense"
#include "Resampled.hpp"
#include "utils.hpp"
#include <type_traits>

namespace LQCDA {

    namespace {
	Eigen::MatrixXd CovProd(double u, double v)
	{
	    return Eigen::MatrixXd::Constant(1, 1, u*v);
	}
	Eigen::MatrixXd CovProd(double u, const std::vector<double>& v)
	{
	    unsigned int m = v.size();
	    Eigen::MatrixXd res(1, m);

	    for(int j = 0; j < m; ++j)
		res(1, j) = u * v[j];
	    
	    return res;
	}
	Eigen::MatrixXd CovProd(const std::vector<double>& u, double v)
	{
	    unsigned int n = u.size();
	    Eigen::MatrixXd res(n, 1);

	    for(int i = 0; i < n; ++i)
		res(i, 1) = u[i] * v;
	    
	    return res;
	}
	Eigen::MatrixXd CovProd(const std::vector<double>& u, const std::vector<double>& v)
	{
	    unsigned int n = u.size();
	    unsigned int m = v.size();
	    Eigen::MatrixXd res(n, m);

	    for(int i = 0; i < n; ++i)
		for(int j = 0; j < m; ++j)
		    res(i, j) = u[i] * v[j];
	    
	    return res;
	}
    }

    template<class U, class V,
	     template<class> class Resampler>
    Eigen::MatrixXd Covariance(const Resampled<U, Resampler>& x, const Resampled<V, Resampler>& y)
    {
	unsigned int n = SizeInfo(x);
	unsigned int m = SizeInfo(y);

	std::vector<U> xtilda = x.Sample() - Resampler<U>::Mean(x.Sample());
	std::vector<V> ytilda = y.Sample() - Resampler<V>::Mean(y.Sample());

	Eigen::MatrixXd res = CovProd(xtilda[0], ytilda[0]);
	
	unsigned int nSample = xtilda.size();
	for(unsigned int s = 1; s < nSample; ++s) {
	    res += CovProd(xtilda[s], ytilda[s]);
	}

	return res / nSample;
    }

} // namespace LQCDA

#endif	// COVARIANCE_FUNCTIONS_HPP_
