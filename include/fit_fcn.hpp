/*
 * fit_fcn.hpp
 *
 *  Created on: Apr 24, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_FCN_HPP_
#define FIT_FCN_HPP_

#include "fit_data.hpp"
#include "models.hpp"
#include "fit_base.hpp"
#include "models.hpp"
#include "Minuit2/FCNBase.h"
#include "Eigen/Dense"
#include "io_utils.hpp"

using namespace ROOT;
using namespace Minuit2;

namespace LQCDA {

/*
 * class Chi2Base
 * Base class to fit data with given model through minimizing a chi-squared
 * function of the data and parameters.
 * To be used with a Minuit2 minimizer.
 */
    class Chi2Base : public FCNBase
    {
    protected:
	FitDataBase* _FitData;	// data to be fitted
	FitModel* _FitModel;		// model used for the fit

	Eigen::MatrixXd _m_C_inv_yy, _m_C_inv_xx, _m_C_inv_xy; // Inverse blocks of the covariance matrix
	
    public:
	Chi2Base(FitDataBase* data, FitModel* model) :
	    _FitData(data), _FitModel(model)
	    {
		compute_C_inv(_FitData);
	    }

	virtual double Up () const { return 1.0; }
	virtual double operator() (const std::vector<double>& params) const;

	double LastValue() const { return last_value; }
	double DOF() const;
	
    private:
	// Compute inverses of covariance matrices
	void compute_C_inv(FitDataBase* data);

	mutable double last_value;
    };

    std::ostream& operator<< (std::ostream& os, const Chi2Base& f);

} // namespace LQCDA

#endif	// FIT_FCN_HPP_