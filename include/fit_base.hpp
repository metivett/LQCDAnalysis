/*
 * fit_base.hpp
 *
 *  Created on: May 21, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_BASE_HPP_
#define FIT_BASE_HPP_

#include "models.hpp"
#include "model_parameters.hpp"
#include "fit_data.hpp"
#include "fit_utils.hpp"
#include "statistics.hpp"
#include "io_utils.hpp"
#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnSimplex.h"
#include "Minuit2/MnScan.h"
#include "Minuit2/ScanMinimizer.h"
#include "Minuit2/MnPlot.h"
#include "Minuit2/MnUserParameters.h"
#include <sstream>
#include <cmath>
#include <utility>

using namespace ROOT;
using namespace Minuit2;

namespace LQCDA {
    
    // TODO Extend to support several minimizers
    template<class Fcn>
    class FitBase
    {
    protected:
	FitModel* _Model;
	FitDataBase* _Data;
	
	bool _IsComputed;
	static const double InitError;
	
	ModelParameters _ModelParameters;
	ModelParameters _DummyParameters;

    public:
	FitBase(FitDataBase* data, FitModel* model);
	FitBase(FitDataBase* data, FitModel* model, const std::vector<double>& initParams);
	FitBase(FitDataBase* data, FitModel* model, const std::vector<double>& initParams, const std::vector<double>& initErrors);

	void SetInitialModelParams(const std::vector<double>& initPar);
	void SetInitialModelParams(const std::vector<double>& initPar, const std::vector<double>& initErr);

	virtual void Evaluate() =0;
	
	template<class Plotter>
	void Plot(unsigned int yk, unsigned int xk);

	double GetFittedParameter(unsigned int n);
	double GetFittedError(unsigned int n);
	std::vector<double> GetPointToModelDistances();

	const FitDataBase* GetFitData() const;
	const FitModel* GetFitModel() const;

	unsigned int NbOfParameters() const;
	
	virtual void PrintParameters();

    protected:
	std::vector<std::pair<double,double> > Fit_impl(const Fcn& F);

    private:
	void SetInitialDummyParameters(FitDataBase* data);
	
    };

    
    template<class Fcn>
    const double FitBase<Fcn>::InitError = 0.1;

// ----------------------------------------------------------------------
// Fit constructors and member functions
// ----------------------------------------------------------------------
    template<class Fcn>
    FitBase<Fcn>::FitBase(FitDataBase* data, FitModel* model) :
	_Model(model), _Data(data),
	_IsComputed(false),
	_ModelParameters(model->NbOfParameters())
    {
	SetInitialDummyParameters(data);
    }

    template<class Fcn>
    FitBase<Fcn>::FitBase(FitDataBase* data, FitModel* model, const std::vector<double>& initParams) :
	_Model(model), _Data(data),
	_IsComputed(false),
	_ModelParameters(initParams)
    {
	assert(initParams.size() == model->NbOfParameters());
	SetInitialDummyParameters(data);
    }

    template<class Fcn>
    FitBase<Fcn>::FitBase(FitDataBase* data, FitModel* model, const std::vector<double>& initParams, const std::vector<double>& initErrors) :
	_Model(model), _Data(data),
	_IsComputed(false),
	_ModelParameters(initParams, initErrors)
    {
	assert(initParams.size() == model->NbOfParameters());
	SetInitialDummyParameters(data);
    }

    template<class Fcn>
    void FitBase<Fcn>::SetInitialModelParams(const std::vector<double>& initPar)
    {
	_ModelParameters = ModelParameters(initPar);
	_IsComputed = false;
    }
    template<class Fcn>
    void FitBase<Fcn>::SetInitialModelParams(const std::vector<double>& initPar, const std::vector<double>& initErr)
    {
	_ModelParameters = ModelParameters(initPar, initErr);
	_IsComputed = false;
    }

    template<class Fcn>
    template<class Plotter>
    void FitBase<Fcn>::Plot(unsigned int yk, unsigned int xk)
    {
	Plotter plot;
	plot.Plot();
    }


    template<class Fcn>
    double FitBase<Fcn>::GetFittedParameter(unsigned int n)
    {
	assert(n < _Model->NbOfParameters());
	if(!_IsComputed) {
	    LQCDOut<<"\nFit results not computed !\n";
	}
	return _ModelParameters.Value(n);
    }
    
    template<class Fcn>
    double FitBase<Fcn>::GetFittedError(unsigned int n)
    {
	assert(n < _Model->NbOfParameters());
	if(!_IsComputed) {
	    LQCDOut<<"\nFit results not computed !\n";
	}
	return _ModelParameters.Error(n);
    }

    template<class Fcn>
    std::vector<double> FitBase<Fcn>::GetPointToModelDistances()
    {
	if(_IsComputed) {
	    size_t nyDim = _Model->YDim();
	    size_t nData = _Data->nData();
	    std::vector<double> res(nData);
	    
	    for(size_t i=0; i<nData; ++i) {
		double d = 0.;
		std::vector<double> y_model = _Model->Eval(_Data->x(i), _ModelParameters);
		for(size_t k=0; k<nyDim; ++k) {
		    double tmp = _Data->y(i,k) - y_model[k];
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

    template<class Fcn>
    const FitDataBase* FitBase<Fcn>::GetFitData() const
    {
	return _Data;
    }
    template<class Fcn>
    const FitModel* FitBase<Fcn>::GetFitModel() const
    {
	return _Model;
    }

    template<class Fcn>
    unsigned int FitBase<Fcn>::NbOfParameters() const
    {
	return _ModelParameters.Size() + _DummyParameters.Size();
    }

    template<class Fcn>
    void FitBase<Fcn>::PrintParameters()
    {
	if(_IsComputed) {
	    LQCDOut<<"\nFitted parameters:\n";
	    LQCDOut << _ModelParameters
		    << '\n';
	}
	else {
	    LQCDOut<<"\nFit results not computed !\n";
	}
    }



    inline MnUserParameters getMnUserParameters(FitDataBase* fitdata, const ModelParameters& initParams, const ModelParameters& dummyParams, double initError)
    {
	const std::vector<FunctionParameter>& params = initParams.Parameters();
	const std::vector<FunctionParameter>& dumparams = dummyParams.Parameters();
	int nxDim = fitdata->nxDim();
	int nyDim = fitdata->nyDim();
	int nData = fitdata->nData();
	MnUserParameters MnInitPar;
	
	for(int p=0; p<params.size(); ++p) {
	    // Add parameter p
	    if(params[p].Error() == 0.) {
		double error = abs(params[p].Value()) * initError;
		MnInitPar.Add(params[p].Name(), params[p].Value(), error);
		MnInitPar.SetError(p, error);
	    }
	    else {
		double error = params[p].Error();
		MnInitPar.Add(params[p].Name(), params[p].Value(),error);
		MnInitPar.SetError(p, error);
	    }
	    
	    // Set optional limits on parameters
	    if(params[p].HasLimits()) {
		if(params[p].HasLowerLimit()) {
		    if(params[p].HasUpperLimit()) {
			MnInitPar.SetLimits(p, params[p].LowerLimit(), params[p].UpperLimit());
		    }
		    else {
			MnInitPar.SetLowerLimit(p, params[p].LowerLimit());
		    }
		}
		else {
		    MnInitPar.SetUpperLimit(p, params[p].UpperLimit());
		}
	    }
	}

	// Set dummy parameters
	for(int p=0; p<dumparams.size(); ++p) {
	    // Add parameter p
	    if(dumparams[p].Error() == 0.) {
		double error = abs(dumparams[p].Value()) * initError;
		MnInitPar.Add(dumparams[p].Name(), dumparams[p].Value(), error);
		MnInitPar.SetError(p+params.size(), error);
		MnInitPar.SetLimits(p+params.size(), dumparams[p].Value() - error, dumparams[p].Value() + error);
	    }
	    else {
		double error = dumparams[p].Error();
		MnInitPar.Add(dumparams[p].Name(), dumparams[p].Value(), error);
		MnInitPar.SetError(p+params.size(), error);
		MnInitPar.SetLimits(p+params.size(), dumparams[p].Value() - error, dumparams[p].Value() + error);
	    }
	}
	
	// Print some information
	LQCDDebug(1)<<"\nFit dimensions :\n"
		    <<"nxDim = "<<nxDim<<'\n'
		    <<"nyDim = "<<nyDim<<'\n'
		    <<"nData = "<<nData<<'\n';
	LQCDDebug(1)<<"\nHaveXCorrelation = "<<fitdata->HaveXCorrelation()<<'\n';

	// Return MnInitPar
	return MnInitPar;
    }

    // FitBase<Fcn>::FitImpl() member function
    template<class Fcn>
    std::vector<std::pair<double,double> > FitBase<Fcn>::Fit_impl(const Fcn& F)
    {
	LQCDDebug(3)<<"\nMinimization function initialized!\n";

	MnUserParameters init_par = getMnUserParameters(_Data, _ModelParameters, _DummyParameters, FitBase<Fcn>::InitError);
	LQCDOut<<"Initial parameters : "<<'\n'<<init_par;
	
	// Pre-minimizer call
	MnMigrad Migrad1(F, init_par, 2);
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
	    if(false) {
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

	    // Return vector < pair(value, error) >
	    int nParams = init_par.Params().size();
	    std::vector<std::pair<double, double> > result(nParams);
	    for(int i = 0; i < nParams; ++i) {
		result[i].first = Min.UserParameters().Value(i);
		result[i].second = Min.UserParameters().Error(i);
	    }
	    return result;
	}
    }

    template<class Fcn>
    void FitBase<Fcn>::SetInitialDummyParameters(FitDataBase* data)
    {
	int nxDim = data->nxDim();
	int nData = data->nData();
	// Add "pseudo-parameters" to deal with cases of x-correlation in dimension k
	for(int k=0; k<nxDim; ++k) {
	    if(data->IsCorrelatedXDim(k)) {
		for(int i=0; i<nData; ++i) {
		    // Name for parameter x_ik
		    std::ostringstream oss;
		    oss<<"x"<<i<<k;
		    // Add parameter p
		    _DummyParameters.Add(oss.str(), data->x(i,k), abs(data->x(i,k)) * FitBase<Fcn>::InitError*0.0001);
		}
	    }
	}
    }


}	// namespace LQCDA


#endif /* FIT_HPP_ */
