/*
 * CovariancePolicies.hpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Thibaut Metivet
 */

#ifndef COVARIANCE_POLICIES_HPP_
#define COVARIANCE_POLICIES_HPP_

#include <vector>
#include "Eigen/Dense"
#include "SizeInfo.hpp"
#include "CovarianceFunctions.hpp"

namespace LQCDA {

    template<class U, class V>
    class FullCovariance
    {
    public:
	static Eigen::MatrixXd Covariance(const std::vector<U>& x, const std::vector<V>& y) {
	    unsigned int nData = x.size();
	    assert(x.size() == y.size());
	    
	    unsigned int xDim = SizeInfo(x[0]);
	    unsigned int yDim = SizeInfo(y[0]);
	    Eigen::MatrixXd C(xDim*nData, yDim*nData);
	    for(int i=0; i<nData; ++i) {
		for(int j=0; j<nData; ++j) {
		    C.block(i * xDim, j * yDim, xDim, yDim) = LQCDA::Covariance(x[i],x[j]);
		}
	    }
	
	    return C;
	}
    };

    template<class U, class V>
    class Identity
    {
    public:
	static Eigen::MatrixXd Covariance(const std::vector<U>& x, const std::vector<V>& y) {
	    unsigned int nData = x.size();
	    assert(x.size() == y.size());
	    
	    unsigned int xDim = SizeInfo(x[0]);
	    unsigned int yDim = SizeInfo(y[0]);
	
	    return Eigen::MatrixXd::Identity(xDim*nData, yDim*nData);
	}
    };

    template<class U, class V>
    class Zero
    {
    public:
	static Eigen::MatrixXd Covariance(const std::vector<U>& x, const std::vector<V>& y) {
	    unsigned int nData = x.size();
	    assert(x.size() == y.size());
	    
	    unsigned int xDim = SizeInfo(x[0]);
	    unsigned int yDim = SizeInfo(y[0]);
	
	    return Eigen::MatrixXd::Zero(xDim*nData, yDim*nData);
	}
    };

} // namespace LQCDA

#endif	// COVARIANCE_POLICIES_HPP_
