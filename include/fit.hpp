/*
 * fit.hpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_HPP_
#define FIT_HPP_

#include "models.hpp"
#include "statistics.hpp"
#include "Minuit2/FCNBase.h"
#include "Eigen/Dense"
#include <sstream>
#include <cmath>

using namespace ROOT;
using namespace Minuit2;

namespace LQCDA {

    class Chi2Base;
    
    class FitData
    {
    protected:
	std::vector<std::vector<double> > _m_data;	// data ("y-points") to be fitted
	std::vector<std::vector<double> > _m_x;	// "x-points" ie points s.t. we evalutate model(x,params)

	std::vector<bool> _is_x_corr, _is_y_corr, _is_xy_corr;

	friend class Chi2Base;
    public:
	// TODO
	FitData(const std::vector<std::vector<double> >& data,
		const std::vector<std::vector<double> >& x,
		const std::vector<bool>& y_corr =std::vector<bool>(),
		const std::vector<bool>& x_corr =std::vector<bool>(),
		const std::vector<bool>& xy_corr =std::vector<bool>())
	    : _m_data(data), _m_x(x), _is_x_corr(x_corr), _is_y_corr(y_corr),
	      _is_xy_corr(xy_corr) {}

	// Getters
	int nData() const { return _m_data.size(); }
	int nxDim() const { return _m_x[0].size(); }
	int xSize() const {
	    return nData() * nxDim();
	}
	int nyDim() const { return _m_data[0].size(); }
	int ySize() const {
	    return nData() * nyDim();
	}
	bool is_x_corr(size_t k) const {
	    if(_is_x_corr.size()==0)
		return false;
	    else
		return _is_x_corr[k];
	}
	bool have_x_corr() const {
	    if(_is_x_corr.size()==0)
		return false;
	    else
		return true;
	}
	bool is_y_corr(size_t k) const {
	    if(_is_y_corr.size()==0)
		return false;
	    else
		return _is_y_corr[k];
	}
	bool have_y_corr() const {
	    if(_is_y_corr.size()==0)
		return false;
	    else
		return true;
	}
    };

/*
 * class Chi2Base
 * Base class to fit data with given model through minimizing a chi-squared
 * function of the data and parameters.
 * To be used with a Minuit2 minimizer.
 */
    class Chi2Base : public FCNBase
    {
    protected:
	const FitData& _m_fitdata;	// data to be fitted
	const FitModel& _m_model;		// model used for the fit

	Eigen::MatrixXd _m_C_inv_yy, _m_C_inv_xx, _m_C_inv_xy; // Inverse blocks of the covariance matrix
	
    public:
	Chi2Base(const FitData& data, const FitModel& model,
		 const Eigen::MatrixXd& C_yy,
		 const Eigen::MatrixXd& C_xx,
		 const Eigen::MatrixXd& C_xy)
	    : _m_data(data), _m_model(model)
	    {
		compute_C_inv(C_yy, C_xx, C_xy);
	    }

	virtual double Up () { return 1.0; }
	virtual double operator() (const std::vector<double>& params);
    private:
	// Bloc inversion:
	// ( A B )-1   ( X      -XBD-1        )
	// ( C D )   = ( -D-1CX D-1+D-1CXBD-1 )
	// with X=(A-BD-1C)-1
	void compute_C_inv(const Eigen::MatrixXd& C_yy,
			   const Eigen::MatrixXd& C_xx,
			   const Eigen::MatrixXd& C_xy)
	    {
		_m_C_inv_yy.noalias() = C_yy.inverse();
		if(C_xy.isZero()) {
		    _m_C_inv_xx.noalias() = C_xx.inverse();
		    _m_C_inv_xy.noalias() = Eigen::MatrixXd::Zero(C_xy.rows(), C_xy.cols());
		}
		else {
		    _m_C_inv_xx.noalias() = (C_xx - C_xy * _m_C_inv_yy * C_xy.transpose()).inverse();
		    _m_C_inv_xy.noalias() = - _m_C_inv_xx * C_xy * _m_C_inv_yy;
		}
		
	    }
    };

    template<class Fcn =Chi2Base>
    class Fit
    {
    private:
	const FitData& _fitdata;
	const FitModel& _model;

	bool _isComputed;

	static const double InitError = 0.5;
    public:
	Fit(const FitData& data, const FitModel& model, bool computeFit =false)
	    : _fitdata(data), _model(model), _isComputed(computeFit)
	    {
		// TODO : checks for coherence
	    }

	void fit(const std::vector<double>& initPar, const std::vector<Limits>& parLimits);
    protected:
    };

    /*
     * Utility classes to deal with parameters limits
     */
    class LimitBase
    {
    protected:
	double limit;
    public:
	LimitBase(double lim)
	    : limit(lim) {}

	double limit() { return limit; }
	virtual void apply(MnUserParameters& params, size_t p) =0;
    };
    class LowerLimit : public LimitBase
    {
    public:
	LowerLimit(double l)
	    : LimitBase(l) {}
    };
    class UpperLimit : public LimitBase
    {
	UpperLimit(double u)
	    : LimitBase(u) {}
    };
    class Limit : public LowerLimit, public UpperLimit
    {
    public:
	Limit(double l, double u)
	    : LowerLimit(l), UpperLimit(u) {}

	double lower() { return LowerLimit::limit(); }
	double upper() { return UpperLimit::limit(); }
    };

    template<class LimitType>
    void setLimit(MnUserParameters& params, size_t p, const LimitBase& limit)
    {
	throw UndefinedLimitType("setLimit()");
    }
    template<>
    void setLimit<LowerLimit>(MnUserParameters& params, size_t p, const LimitBase& limit)
    {
	params.SetLowerLimit(p, limit.limit());
    }
    template<>
    void setLimit<UpperLimit>(MnUserParameters& params, size_t p, const LimitBase& limit)
    {
	params.SetUpperLimit(p, limit.limit());
    }
    template<>
    void setLimit<Limit>(MnUserParameters& params, size_t p, const LimitBase& limit)
    {
	params
    }

    template<class Fcn>
    void Fit<Fcn>::fit(const std::vector<double>& initPar,
		       const std::vector<LimitBase>& parLimits)
    {
	size_t nParModel = _model.nParams();
	MnUserParameters init_par;
	
	if(initPar.size() != nParModel) {
	    throw OutOfRange("Fit::fit()", nParModel);
	}
	for(int p=0; p<nParModel; ++p) {
	    // Name for parameter p
	    std::string parName = _model.getParName(p);
	    if(parName == "") {
		std::istringstream iss;
		iss<<"p"<<p;
		parName = iss.str();
	    }
	    // Add parameter p
	    init_par.Add(parName.c_str(), initPar[p], abs(iniPar[p]) * InitError);

	    // Set optional limits on parameters
	    if(parLimits.size() == nParModel) {
		
	    }
	    else if(parLimits.size() != 0) {
		throw OutOfRange("Fit::fit()", parLimits.size());
	    }
	}
	
	
    }

}	// namespace LQCDA


#endif /* FIT_HPP_ */
