/*
 * fit_resampled.hpp
 *
 *  Created on: Apr 16, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_RESAMPLED_HPP_
#define FIT_RESAMPLED_HPP_

#include "fit_base.hpp"
#include "model_parameters.hpp"

namespace LQCDA {

    template <class Fcn, template <typename T> class Resampler>
    class ResampledFit : public FitBase<Fcn>
    {
    protected:
	using FitBase<Fcn>::_Model;
	using FitBase<Fcn>::_Data;
	using FitBase<Fcn>::_IsComputed;
	using FitBase<Fcn>::_ModelParameters;
	using FitBase<Fcn>::_DummyParameters;

	using FitBase<Fcn>::Fit_impl;
	
    public:
	ResampledFit(ResampledFitData<Resampler>* data, FitModel* model) :
	    FitBase<Fcn>(data,model) {}
	ResampledFit(ResampledFitData<Resampler>* data, FitModel* model, const std::vector<double>& initParams) :
	    FitBase<Fcn>(data, model, initParams) {}
	ResampledFit(ResampledFitData<Resampler>* data, FitModel* model, const std::vector<double>& initParams, const std::vector<double>& initErrors) :
	    FitBase<Fcn>(data, model, initParams, initErrors) {}

	virtual void Evaluate();
    };

    
// virtual ResampledFit::Evaluate() member function
    template<class Fcn, template <typename T> class Resampler>
    void ResampledFit<Fcn, Resampler>::Evaluate()
    {
	Fcn F(_Data, _Model);

	ResampledFitData<Resampler>* data = dynamic_cast<ResampledFitData<Resampler>* > (_Data);
        // Fit each resampled sample
//	for(int sample=0; sample<_fitdata->nSample(); ++sample) {
	for(int sample = 0; sample < 1; ++sample) {
	    data->SetCurrentSample(sample);
	    std::vector<std::pair<double,double> > Result = Fit_impl(F);

	    // Get fitted values
	    unsigned int nmodelpar = _ModelParameters.Size();
	    for(int i = 0; i < nmodelpar; ++i) {
	    _ModelParameters.SetValue(i, Result[i].first);
	    _ModelParameters.SetError(i, Result[i].second);
	    }
	    for(int i = _ModelParameters.Size(); i < Result.size(); ++i) {
		_DummyParameters.SetValue(i-nmodelpar, Result[i].first);
		_DummyParameters.SetError(i-nmodelpar, Result[i].second);
	    }
	}

	_IsComputed = true;
    }

} // namespace LQCDA

#endif	// FIT_RESAMPLED_HPP_
