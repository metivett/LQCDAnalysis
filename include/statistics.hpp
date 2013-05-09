/*
 * statistics.hpp
 *
 *  Created on: Feb 23, 2013
 *      Author: Thibaut Metivet
 */

#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_

#include <gsl/gsl_cdf.h>


namespace LQCDA {
    
    /*   // CHECK
    template<typename T, typename Function>
    std::vector<typename Function::return_type> resample(const std::vector<T>& dat,
							 Resampler<T> sampler, unsigned int n_resample, Function f) {
	std::vector<typename Function::return_type> result(n_resample+1);
	result[0] = f(dat);
	for(int i=1; i<n_resample+1; ++i)
	    result[i] = f(sampler.resample(dat));

	return result;
	}*/

    
/*
 * Statistical functions to deal with samples of vectors
 */
    template<typename T>
    T SampleVariance (const std::vector<T>& v)
    {
	std::vector<T> x = v - mean(v);
	return mean(x*x);
    }
    template<typename T>
    T SampleCovariance (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	int m = w.size();
	if(n != m)
	    throw DataException("You're computing covariance with sample vectors of different sizes!");
	std::vector<T> x = v - mean(v);
	std::vector<T> y = w - mean(w);
	return mean(x*y);
    }
    template<typename T>
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> covariance (const std::vector<T>& x, const std::vector<T>& y)
    {    
	if(&x == &y)
	    return Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>::Identity(x.size(),x.size());
	else
	    return Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>::Zero(x.size(),y.size());
    }
    template<typename Scalar, int Rows, int Cols>
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> covariance(
	const std::vector<Eigen::Array<Scalar, Rows, Cols> >& v,
	const std::vector<Eigen::Array<Scalar, Rows, Cols> >& w) {
	int n = v.size();
	int m = w.size();
	if(n != m)
	    throw DataException("You're computing covariance with sample vectors of different sizes!");
	std::vector<Eigen::Array<Scalar, Rows, Cols> > x = v - mean(v);
	std::vector<Eigen::Array<Scalar, Rows, Cols> > y = w - mean(w);
	std::vector<Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> > result(n);

	struct v_times_w_transpose {
	    v_times_w_transpose() {}
	    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> operator() (
		const Eigen::Array<Scalar, Rows, Cols>& a,
		const Eigen::Array<Scalar, Rows, Cols>& b) {
		return a.matrix() * b.matrix().transpose();
	    }
	};
	std::transform(x.begin(), x.end(), y.begin(), result.begin(), v_times_w_transpose());
	return mean(result);
    }

/*
 * Chi2 p-value given chi2_val value
 * ie returns integral(p(x)dx, x=chi2_val..infinity) with p chi2 distribution
 */
    inline double chi2_pvalue(const double chi2_val, const size_t ndof)
    {
	return gsl_cdf_chisq_Q(chi2_val,(double)ndof);
    }


}	// namespace LQCDA


#endif /* STATISTICS_HPP_ */
