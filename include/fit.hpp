/*
 * fit.hpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_HPP_
#define FIT_HPP_

#include "fit_base.hpp"

namespace LQCDA {

    template<class Fcn>
    class Fit : public FitBase<Fcn>
    {
    protected:
	using FitBase<Fcn>::_Model;
	using FitBase<Fcn>::_Data;
	using FitBase<Fcn>::_IsComputed;
	using FitBase<Fcn>::_ModelParameters;
	using FitBase<Fcn>::_DummyParameters;

	using FitBase<Fcn>::Fit_impl;
	
    public:
	Fit(FitData* data, FitModel* model) :
	    FitBase<Fcn>(data, model) {}
	Fit(FitData* data, FitModel* model, const std::vector<double>& initParams) :
	    FitBase<Fcn>(data, model, initParams) {}
	Fit(FitData* data, FitModel* model, const std::vector<double>& initParams, const std::vector<double>& initErrors) :
	    FitBase<Fcn>(data, model, initParams, initErrors) {}

	virtual void Evaluate();

	virtual void PrintParameters();
	
    };
    
    template<class Fcn>
    void Fit<Fcn>::Evaluate()
    {	
	Fcn F(_Data, _Model);
	
        // Fit
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
	
	_IsComputed = true;
    }

}	// namespace LQCDA


#endif /* FIT_HPP_ */
