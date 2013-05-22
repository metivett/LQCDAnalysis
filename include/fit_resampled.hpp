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

	using FitBase<Fcn>::Fit_impl;
	
    public:
	ResampledFit(ResampledFitData<Resampler>* data, FitModel* model)
	    : FitBase<Fcn>(data,model)
	    {}

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
	    data->setCurrentSample(sample);
	    std::vector<std::pair<double,double> > Result = Fit_impl(F);
    
	    // Get fitted values
	    for(int i = 0; i < Result.size(); ++i) {
		_ModelParameters.SetValue(i, Result[i].first);
		_ModelParameters.SetError(i, Result[i].second);
	    }
	}

	_IsComputed = true;
    }

} // namespace LQCDA

#endif	// FIT_RESAMPLED_HPP_
