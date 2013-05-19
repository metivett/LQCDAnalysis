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
#include <assert.h>
#include "model_parameters.hpp"

namespace LQCDA {

    class ModelFunction
    {
    public:
	virtual double operator() (const std::vector<double>& x, const std::vector<double>& params) const =0;
	virtual unsigned int XDim() const =0;
	virtual unsigned int NbOfParameters() =0;
    };

    class FitModel
    {
    protected:
	std::string _Name;
	
	std::vector<ModelFunction*> _ModelFunctions;

	std::vector<std::vector<bool> > _FunctionMasks;
	
	size_t _NbOfParameters;
	size_t _XDim, _YDim;
	
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
                _XDim = max(_Xdim,_ModelFunctions[k]->XDim());
            }
        }

	std::string Name() const { return _Name; }
	size_t NbOfParameters() const { return _NbOfParameters; }

	size_t XDim() const { return _XDim; }
	size_t YDim() const { return _YDim; }

	double Eval(size_t k, const std::vector<double>& x, const ModelParameters& params) const
	    {
		assert(k < _YDim);
		else return (*_m_fcts[k])(x, params.ParamValues(_FunctionMasks[k]));
	    }
	std::vector<double> Eval(const std::vector<double>& x, const ModelParameters& params) const
	    {
		std::vector<double> result(_YDim);
		for(int k = 0; k < _YDim; ++k)
		    result[k] = (*_m_fcts[k])(x, params.ParamValues(_FunctionMasks[k]));

		return result;
	    }
    };
    

}	// namespace LQCDA


#endif /* MODELS_HPP_ */
