/*
 * fit.hpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_HPP_
#define FIT_HPP_

#include "models.hpp"
#include "fit_data.hpp"
#include "fit_utils.hpp"
#include "statistics.hpp"
#include "io_utils.hpp"
#include "exceptions.hpp"
#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnScan.h"
#include "Minuit2/ScanMinimizer.h"
#include "Minuit2/MnPlot.h"
#include "Minuit2/MnUserParameters.h"
#include "Eigen/Dense"
#include <sstream>
#include <cmath>
#include <utility>

using namespace ROOT;
using namespace Minuit2;

namespace LQCDA {
    
    class FitBase
    {
    protected:
	FitModel* _model;
	
	bool _isComputed;
	const double InitError = 0.5;
	
	size_t _nFittedParams;

    public:
	FitBase(FitModel* model)
	    : _model(model), _isComputed(false),
	      _nFittedParams(model->nParams())
	    {}

	size_t nFittedParams() { return _nFittedParams; }

	virtual double getFittedParameter(unsigned int n) =0;
	virtual double getFittedError(unsigned int n) =0;
	
	void printParameters();
    };

    inline void FitBase::printParameters()
    {
	if(_isComputed) {
	    LQCDOut<<"\nFitted parameters:\n";
	    for(int i=0; i<_nFittedParams; ++i) {
		LQCDOut<<"p"<<i<<" = "<<getFittedParameter(i)<<" +- "<<getFittedError(i)
		       <<'\n';
	    }
	}
	else {
	    LQCDOut<<"\nFit results not computed !\n";
	}
    }
    
    template<class Fcn>
    class Fit : public FitBase
    {
    private:
	FitDataBase* _fitdata;
//	FunctionMinimum _min;
	vector<double> _fittedParams;
	vector<double> _fittedErrors;
	
    protected:
	bool _isComputed;
	const double InitError = 0.5;	
	
    public:
	Fit(FitDataBase* data, FitModel* model)
	    : FitBase(model), _fitdata(data),
	      _fittedParams(), _fittedErrors(),
	      _isComputed(false)
	    {
		// TODO : checks for coherence
		for(int k=0; k<nxDim; ++k) {
		    if(_fitdata->is_x_corr(k)) {
			_nFittedParams += data->nData();
		    }
		}
		_fittedParams.resize(_nFittedParams);
		_fittedErrors.resize(_nFittedParams);
	    }

	void fit(const std::vector<double>& initPar, const std::vector<LimitBase*>& parLimits);
	void fit() { fit(std::vector<double>(_model->nParams()), std::vector<LimitBase*>()); }

	size_t nFittedParams() { return _nFittedParams; }

	double getFittedParameter(unsigned int n);
	double getFittedError(unsigned int n);
	
	void printParameters();

    private:
	void fit_impl(const std::vector<double>& initPar, const std::vector<LimitBase*>& parLimits);
    };

    // Fit::fit() member function
    template<class Fcn>
    void Fit<Fcn>::fit(const std::vector<double>& initPar,
		       const std::vector<LimitBase*>& parLimits)
    {
	if(!_isComputed) {
	    fit_impl(initPar, parLimits);
	}
    }

    template<class Fcn>
    void Fit<Fcn>::fit_impl(const std::vector<double>& initPar,
			    const std::vector<LimitBase*>& parLimits)
    {
	size_t nParModel = _model->nParams();
	int nxDim = _fitdata->nxDim();
	int nyDim = _fitdata->nyDim();
	int nData = _fitdata->nData();
	MnUserParameters init_par;
	
	if(initPar.size() != nParModel) {
	    throw OutOfRange("Fit::fit()", nParModel);
	}
	
	for(int p=0; p<nParModel; ++p) {
	    // Name for parameter p
	    std::string parName = _model->getParName(p);
	    if(parName == "") {
		std::ostringstream oss;
		oss<<"p"<<p;
		parName = oss.str();
	    }
	    // Add parameter p
	    init_par.Add(parName.c_str(), initPar[p], abs(initPar[p]) * InitError);
	    _nFittedParams++;

	    // Set optional limits on parameters
	    if(parLimits.size() == nParModel) {
		parLimits[p]->apply(init_par, p);
	    }
	    else if(parLimits.size() != 0) {
		throw OutOfRange("Fit::fit()", parLimits.size());
	    }
	}
	
	// Add "pseudo-parameters" to deal with cases of x-correlation in dimension k
	for(int k=0; k<nxDim; ++k) {
	    if(_fitdata->is_x_corr(k)) {
		for(int i=0; i<nData; ++i) {
		    // Name for parameter p
		    std::ostringstream oss;
		    oss<<"x"<<i<<k;
		    // Add parameter p
		    init_par.Add(oss.str().c_str(), _fitdata->x(i,k), abs(_fitdata->x(i,k)) * InitError);
		}
	    }
	}

	LQCDDebug(1)<<"\nFit dimensions :\n"
		    <<"nxDim = "<<nxDim<<'\n'
		    <<"nyDim = "<<nyDim<<'\n'
		    <<"nData = "<<nData<<'\n';
	LQCDDebug(1)<<"\nX-corr = "<<_fitdata->have_x_corr()<<'\n';
	// TODO Extend to support several minimizers

        // Fit each resampled sample
	for(int sample=0; sample<_fitdata->nSample(); ++sample) {
	    _fitdata->setCurrentSample(sample);
	    Fcn F(_fitdata, _model);
	    LQCDDebug(3)<<"\nMinimization function initialized!\n";
	
	    // Pre-minimizer call
	    MnMigrad Migrad1(F, init_par, 0);
	    FunctionMinimum Min = Migrad1();
	    LQCDDebug(3)<<"(MINUIT) Pre-minimizer call :\n"
			<< "--------------------------------------------------------"
			<< Min
			<< "--------------------------------------------------------"
			<< std::endl;
	
	    // Minimizer call
	    MnUserParameters pre_min_par = Min.UserParameters();
	    MnMigrad Migrad2(F, pre_min_par, 2);
	    Min = Migrad2();

	    if(!Min.IsValid()) {
		LQCDOut<<"Minuit Library reported that minimization result is not valid !\n";
		throw MinimizationException();
	    }
	    else {
		_isComputed = true;
		for(int i=0; i<_nFittedParams; ++i) {
		    _fittedParams[i] = Min.UserParameters().Value(i);
		    _fittedErrors[i] = Min.UserParameters().Error(i);
		}
	    }
	    LQCDDebug(3)<<"(MINUIT) Minimizer call :\n"
			<< "--------------------------------------------------------"
			<< Min
			<< "--------------------------------------------------------"
			<< std::endl;

	    /*// Scan around position
	    std::vector<std::pair<double,double> > ScanRes;
	    MnPlot Plot;

	    MnScan Scanner(F, Min.UserParameters(), 2);
	    LQCDOut<<"(MINUIT) Scan around last position :\n"
		   << "--------------------------------------------------------"
		   <<std::endl;
	    for(int i=0; i<init_par.Params().size(); ++i) {
		LQCDOut<<"Parameter p"<<i<<std::endl;
		ScanRes = Scanner.Scan(i);
		Plot(ScanRes);
	    }
	    LQCDOut<< "--------------------------------------------------------"
	           << std::endl;
	    */
	}
    }

    template<class Fcn>
    inline double Fit<Fcn>::getFittedParameter(unsigned int n)
    {
	if(n >= _nFittedParams)
	    throw OutOfRange("Fit<Fcn>::getFittedParameter", n);
	else {
	    return _fittedParameter[n];
	}
    }

    template<class Fcn>
    inline double Fit<Fcn>::getFittedError(unsigned int n)
    {
	if(n >= _nFittedParams)
	    throw OutOfRange("Fit<Fcn>::getFittedParameter", n);
	else {
	    return _fittedErrors[n];
	}
    }

}	// namespace LQCDA


#endif /* FIT_HPP_ */
