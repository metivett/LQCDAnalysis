/*
 * model_parameters.hpp
 *
 *  Created on: May 17, 2013
 *      Author: Thibaut Metivet
 */

#ifndef MODEL_PARAMETERS_HPP_
#define MODEL_PARAMETERS_HPP_

#include "function_parameter.hpp"
#include <vector>

namespace LQCDA {

    class ModelParameters
    {
    private:
	std::vector<FunctionParameter> _FunctionParameters;

    public:
	ModelParameters() : _FunctionParameters() {}
	ModelParameters(size_t nparams);
	ModelParameters(const std::vector<double>& vals, const std::vector<double>& errs);

	// Adds new parameter with name, value, error, lower and upper bounds.
	// Returns false if the parameter already exists.
	bool Add(const std::string& name, double val, double err, double low, double up);
	bool Add(const std::string& name, double val, double err);
	bool Add(const std::string& name, double val);

	void SetValue(unsigned int n, double val);
	void SetError(unsigned int n, double err);
	void SetLimits(unsigned int n, double low, double up);
	void SetLowerLimit(unsigned int n, double low);
	void SetUpperLimit(unsigned int n, double up);
	void RemoveLimits(unsigned int n);

	std::vector<double> ParamValues() const;
	std::vector<double> ParamValues(const std::vector<bool>& mask) const;
	std::vector<double> ParamErrors() const;
	std::vector<double> ParamErrors(const std::vector<bool>& mask) const;
	
	const std::vector<FunctionParameter>& Parameters() const;
	
	double Value(unsigned int n);
	double Error(unsigned int n);
	
    };

}

#endif	// MODEL_PARAMETER_STATE_HPP_
