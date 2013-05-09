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
#include "exceptions.hpp"

namespace LQCDA {

    class ModelFunction
    {
    public:
	virtual double operator() (const std::vector<double>& x, const std::vector<double>& params) const =0;
    };

    class FitModel
    {
    public:
    protected:
	std::string _m_name;
	std::vector<ModelFunction*> _m_fcts;
	size_t _m_nb_params;
	size_t _m_x_dim, _m_y_dim;
    public:
	FitModel(const std::vector<ModelFunction*>& fcts, size_t nparam, size_t xdim, const char* name ="")
	    : _m_name(name), _m_fcts(fcts), _m_nb_params(nparam), _m_x_dim(xdim), _m_y_dim(fcts.size()) {}

	std::string name() const { return _m_name; }
	size_t nParams() const { return _m_nb_params; }
	// TODO
	std::string getParName(size_t p) const { return (std::string)(""); }
	size_t xDim() const { return _m_x_dim; }
	size_t yDim() const { return _m_y_dim; }

	double eval(size_t k, const std::vector<double>& x, const std::vector<double>& params) const
	    {
		if(k >= _m_fcts.size())
		    throw OutOfRange("FitModel::eval()", k);
		else return (*_m_fcts[k])(x, params);
	    }
	std::vector<double> eval(const std::vector<double>& x, const std::vector<double>& params) const
	    {
		std::vector<double> res(_m_fcts.size());
		for(int i=0; i<_m_fcts.size(); ++i)
		    res[i] = (*_m_fcts[i])(x, params);

		return res;
	    }
    };
    

}	// namespace LQCDA


#endif /* MODELS_HPP_ */
