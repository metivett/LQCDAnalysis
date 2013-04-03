/*
 * statistics.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: Thibaut Metivet
 */

#include "statistics.hpp"
#include "exceptions.hpp"

/*
 * RandomVector operators
 */
RandomVector LQCDA::operator+ (const RandomVector& x, const RandomVector& y)
{
    return RandomVector(x.values()+y.values());
}
RandomVector LQCDA::operator+ (const RandomVector& x, double d)
{
    return RandomVector(x.values()+std::vector(x.values().size(), d));
}
RandomVector LQCDA::operator- (const RandomVector& x, const RandomVector& y)
{
    return RandomVector(x.values()-y.values());
}
RandomVector LQCDA::operator- (const RandomVector& x, double d)
{
    return RandomVector(x.values()-std::vector(x.values().size(), d));
}

/*
 * RandomVector utility functions
 */
RandomVariable LQCDA::dotprod (const RandomVector& x, const RandomVector& y)
{
    size_t xsz = x.values().size();
    size_t ysz = y.values().size();
    if(xsz != ysz)
	throw DataException("You're computing dotproduct with RandomVectors of different sizes!");
    RandomVariable result;
    for(int i=0; i<xsz; ++i)
    {
	result += x.values()[i]*y.values()[i];
    }
    return result;
}

/*
 * RandomVector utility statistical functions
 */
Eigen::MatrixXd LQCDA::covariance(const RandomVector& v, const RandomVector& w) {
    int n = v._m_sample.size();
    int m = w._m_sample.size();
    if(n != m)
	throw DataException("You're computing covariance with sample vectors of different sizes!");
    RandomVector x = v - mean(v);
    RandomVector y = w - mean(w);
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
