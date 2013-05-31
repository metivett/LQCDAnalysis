/*
 * models.hpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Thibaut Metivet
 */

#ifndef MODELS_HPP_
#define MODELS_HPP_

#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include "model_parameters.hpp"
#include "io_utils.hpp"

namespace LQCDA {

    class ModelFunction
    {
    public:
	virtual double operator() (const std::vector<double>& x, const std::vector<double>& params) const =0;
	virtual unsigned int XDim() const =0;
	virtual unsigned int NbOfParameters() const =0;
    };

    class FitModel
    {
    protected:
	std::string _Name;
	
	std::vector<ModelFunction*> _ModelFunctions;

	std::vector<std::vector<bool> > _FunctionMasks;
	
	unsigned int _NbOfParameters;
	unsigned int _XDim, _YDim;
	
    public:
        FitModel(const std::string& name, const std::vector<ModelFunction*>& fcts, const std::vector<std::vector<bool> >& masks) :
	    _Name(name),
	    _ModelFunctions(fcts),
	    _FunctionMasks(masks),
	    _NbOfParameters(0),
	    _XDim(0),
	    _YDim(fcts.size())
	{
	    assert(_FunctionMasks.size() == _YDim);
            for(int k = 0; k < _YDim; ++k) {
                _NbOfParameters += _ModelFunctions[k]->NbOfParameters();
                _XDim = std::max(_XDim,_ModelFunctions[k]->XDim());
            }
        }

	std::string Name() const { return _Name; }
	size_t NbOfParameters() const { return _NbOfParameters; }

	unsigned int XDim() const { return _XDim; }
	unsigned int YDim() const { return _YDim; }

	double Eval(size_t k, const std::vector<double>& x, const ModelParameters& params) const {
	    assert(k < _YDim);
	    return (*_ModelFunctions[k])(x, params.ParamValues(_FunctionMasks[k]));
	}
	double Eval(size_t k, const std::vector<double>& x, const std::vector<double>& params) const {
	    assert(k < _YDim);
	    assert(params.size() == _NbOfParameters);
	    std::vector<double> buf;
	    buf.reserve(_NbOfParameters);
	    for(int n = 0; n < _NbOfParameters; ++n) {
		if(_FunctionMasks[k][n])
		    buf.push_back(params[n]);
	    }
	    
	    return (*_ModelFunctions[k])(x, buf);
	}
	std::vector<double> Eval(const std::vector<double>& x, const ModelParameters& params) const {
	    std::vector<double> result(_YDim);
	    for(int k = 0; k < _YDim; ++k)
		result[k] = (*_ModelFunctions[k])(x, params.ParamValues(_FunctionMasks[k]));
	    
	    return result;
	}
	std::vector<double> Eval(const std::vector<double>& x, const std::vector<double>& params) const {
	    std::vector<double> result(_YDim);
	    for(int k = 0; k < _YDim; ++k) {
		std::vector<double> buf;
		buf.reserve(_NbOfParameters);
		for(int n = 0; n < _NbOfParameters; ++n) {
		    if(_FunctionMasks[k][n])
			buf.push_back(params[n]);
		}
		result[k] = (*_ModelFunctions[k])(x, buf);
	    }
	    
	    return result;
	}
    };
    

}	// namespace LQCDA


#endif /* MODELS_HPP_ */
