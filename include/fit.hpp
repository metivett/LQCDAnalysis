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
    
    class Fit
    {
    protected:
	FitModel* _Model;
	
	bool _IsComputed;
	const double InitError = 0.5;
	
	ModelParameters _ModelParameters;
	std::vector<FunctionParameter> _DummyParameters;

    public:
	Fit(FitModel* model);
	Fit(FitModel* model, const std::vector<ModelParam>& initPar);
	Fit(FitModel* model, const std::vector<double>& initPar);

	void SetInitModelParams(const std::vector<ModelParam>& initPar);
	void SetInitModelParams(const std::vector<double>& initPar);

	template<class Fcn>
	void fit(FitData* data);
	template<class Fcn, template <typename T> class Resampler>
	void fit(ResampledFitData<Resampler>* data);

	double getFittedParameter(unsigned int n);
	double getFittedError(unsigned int n);
	virtual std::vector<double> getPointToModelDistances(FitDataBase* data);
	
	virtual void printParameters();

    protected:
	template<class Fcn>
	FunctionMinimum fit_impl(FitDataBase* fitdata, const MnUserParameters& initP, bool scan =false);

	MnUserParameters getMnUserParameters(FitDataBase* fitdata, const std::vector<ModelParam>& initPar);

    private:
	
    };

    // FitBase::fit_impl() member function
    template<class Fcn>
    FunctionMinimum Fit::fit_impl(FitDataBase* fitdata, const MnUserParameters& init_par, bool scan)
    {
	Fcn F(fitdata, _model);
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
	    LQCDDebug(1)<< "(MINUIT) Fit successful !\n"
			<< "Resulting minimum is : "
			<< F
			<< std::endl;
	    LQCDDebug(3)<< "(MINUIT) Minimizer call :\n"
			<< "--------------------------------------------------------"
			<< Min
			<< "--------------------------------------------------------"
			<< std::endl;
	    if(scan) {
		// Scan around position
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
	    }
	    return Min;
	}
    }

    inline MnUserParameters FitBase::getMnUserParameters(FitDataBase* fitdata, const std::vector<ModelParam>& initPar)
    {
	size_t nParModel = _Model->nParams();
	int nxDim = fitdata->nxDim();
	int nyDim = fitdata->nyDim();
	int nData = fitdata->nData();
	MnUserParameters MnInitPar;
	
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
	    MnInitPar.Add(parName.c_str(), initPar[p], abs(initPar[p]) * InitError);

	    // Set optional limits on parameters
	    if(initPar[p].Limit()) {
		parLimits[p]->apply(init_par, p);
	    }
	}
	
	// Add "pseudo-parameters" to deal with cases of x-correlation in dimension k
	for(int k=0; k<nxDim; ++k) {
	    if(fitdata->is_x_corr(k)) {
		for(int i=0; i<nData; ++i) {
		    // Name for parameter p
		    std::ostringstream oss;
		    oss<<"x"<<i<<k;
		    // Add parameter p
		    init_par.Add(oss.str().c_str(), fitdata->x(i,k), abs(fitdata->x(i,k)) * InitError);
		}
	    }
	}
	
	// Print some information
	LQCDDebug(1)<<"\nFit dimensions :\n"
		    <<"nxDim = "<<nxDim<<'\n'
		    <<"nyDim = "<<nyDim<<'\n'
		    <<"nData = "<<nData<<'\n';
	LQCDDebug(1)<<"\nX-corr = "<<fitdata->have_x_corr()<<'\n';
	
	// Return init_par
	return init_par;
    }

    inline double FitBase::getFittedParameter(unsigned int n)
    {
	if(n >= _nFittedParams)
	    throw OutOfRange("Fit<Fcn>::getFittedParameter", n);
	else {
	    return _fittedParams[n];
	}
    }

    inline double FitBase::getFittedError(unsigned int n)
    {
	if(n >= _nFittedParams)
	    throw OutOfRange("Fit<Fcn>::getFittedParameter", n);
	else {
	    return _fittedErrors[n];
	}
    }

    inline std::vector<double> FitBase::getPointToModelDistances(FitDataBase* data)
    {
	if(_isComputed) {
	    size_t nyDim = _model->yDim();
	    size_t nData = data->nData();
	    std::vector<double> res(nData);
	    
	    for(size_t i=0; i<nData; ++i) {
		double d = 0.;
		std::vector<double> y_model = _model->eval(data->x(i), _fittedParams);
		for(size_t k=0; k<nyDim; ++k) {
		    double tmp = data->y(i,k) - y_model[k];
		    d +=  tmp * tmp;
		}
		res[i] = sqrt(d);
	    }
	    return res;
	}
	else {
	    LQCDOut<<"\nFit results not computed !\n";
	    return std::vector<double>();
	}
    }

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
	FitData* _fitdata;	
	
    public:
	Fit(FitData* data, FitModel* model)
	    : FitBase(data,model), _fitdata(data)
	    {
		// TODO : checks for coherence
	    }

	using FitBase::fit;
	void fit(const std::vector<double>& initPar, const std::vector<LimitBase*>& parLimits);

    };
    
    template<class Fcn>
    void Fit<Fcn>::fit(const std::vector<double>& initPar,
		       const std::vector<LimitBase*>& parLimits)
    {	
	// TODO Extend to support several minimizers

        // Fit
	FunctionMinimum Min = fit_impl<Fcn>(_fitdata, getMnUserParameters(_fitdata, initPar, parLimits), true);
	
        // Get fitted values
	for(int i=0; i<_nFittedParams; ++i) {
	    _fittedParams[i] = Min.UserParameters().Value(i);
	    _fittedErrors[i] = Min.UserParameters().Error(i);
	}
	_isComputed = true;
    }

}	// namespace LQCDA


#endif /* FIT_HPP_ */
