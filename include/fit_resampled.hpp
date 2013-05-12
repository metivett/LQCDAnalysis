/*
 * fit_resampled.hpp
 *
 *  Created on: Apr 16, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_RESAMPLED_HPP_
#define FIT_RESAMPLED_HPP_

#include "fit.hpp"

namespace LQCDA {

    template <class Fcn, template <typename T> class Resampler>
    class ResampledFit : public FitBase
    {
    private:
        ResampledFitData<Resampler>* _fitdata;

    public:
	ResampledFit(ResampledFitData<Resampler>* data, FitModel* model)
	    : FitBase(data,model), _fitdata(data)
	    {}

	using FitBase::fit;
	void fit(const std::vector<double>& initPar, const std::vector<LimitBase*>& parLimits);
	void fit_sample(unsigned int k,
			const std::vector<double>& initPar, const std::vector<LimitBase*>& parLimits);
	void fit_sample(unsigned int k) {
	    fit_sample(k, std::vector<double>(_model->nParams()), std::vector<LimitBase*>());
	}
    };

    
// ResampledFit::fit() member function
    template<class Fcn, template <typename T> class Resampler>
    void ResampledFit<Fcn,Resampler>::fit(const std::vector<double>& initPar,
			    const std::vector<LimitBase*>& parLimits)
    {	
	// TODO Extend to support several minimizers

	MnUserParameters init_par = getMnUserParameters(_fitdata, initPar, parLimits);
	std::vector<std::vector<double> > fittedP(_nFittedParams), fittedE(_nFittedParams);
	
        // Fit each resampled sample
	for(int sample=0; sample<_fitdata->nSample(); ++sample) {
	    _fitdata->setCurrentSample(sample);
	    FunctionMinimum Min = fit_impl<Fcn>(_fitdata, init_par);
	    
	    for(int i=0; i<_nFittedParams; ++i) {
		fittedP[i].push_back(Min.UserParameters().Value(i));
		fittedE[i].push_back(Min.UserParameters().Error(i));
	    }
	}

	// Get fitted values
	for(int i=0; i<_nFittedParams; ++i) {
	    _fittedParams[i] = Resampler<double>::mean(fittedP[i]);
	    _fittedErrors[i] = Resampler<double>::mean(fittedE[i]);
	}
	_isComputed = true;
    }

    // ResampledFit::fit_sample() member function
    template<class Fcn, template <typename T> class Resampler>
    void ResampledFit<Fcn,Resampler>::fit_sample(unsigned int k,
						 const std::vector<double>& initPar,
						 const std::vector<LimitBase*>& parLimits)
    {	
	// TODO Extend to support several minimizers

	MnUserParameters init_par = getMnUserParameters(_fitdata, initPar, parLimits);
	
        // Fit sample k
	_fitdata->setCurrentSample(k);
	FunctionMinimum Min = fit_impl<Fcn>(_fitdata, init_par);

	// Get fitted values
	for(int i=0; i<_nFittedParams; ++i) {
	    _fittedParams[i] = Min.UserParameters().Value(i);
	    _fittedErrors[i] = Min.UserParameters().Error(i);
	}
	_isComputed = true;
    }

} // namespace LQCDA

#endif	// FIT_RESAMPLED_HPP_
