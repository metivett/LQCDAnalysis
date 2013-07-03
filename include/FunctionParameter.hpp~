/*
 * function_parameter.hpp
 *
 *  Created on: May 16, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FUNCTION_PARAMETER_HPP_
#define FUNCTION_PARAMETER_HPP_

#include <string>
#include <assert.h>

namespace LQCDA {

    class FunctionParameter
    {
    private:
	unsigned int _NumberId;

	std::string _Name;
	
	double _Value;
	double _Error;
	double _LowerLimit;
	double _UpperLimit;

	bool _HasLowerLimit;
	bool _HasUpperLimit;

    public:
	FunctionParameter(unsigned int num, const std::string& name) :
	    FunctionParameter(num, name, 0.)
	    {}
	FunctionParameter(unsigned int num, const std::string& name, double val) :
	    FunctionParameter(num, name, val, 0.)
	    {}
	FunctionParameter(unsigned int num, const std::string& name, double val, double err) :
	    _NumberId(num),
	    _Name(name),
	    _Value(val),
	    _Error(err),
	    _LowerLimit(0.),
	    _UpperLimit(0.),
	    _HasLowerLimit(false),
	    _HasUpperLimit(false)
	    {}
	FunctionParameter(unsigned int num, const std::string& name, double val, double err, double min, double max) :
	    _NumberId(num),
	    _Name(name),
	    _Value(val),
	    _Error(err),
	    _LowerLimit(min),
	    _UpperLimit(max),
	    _HasLowerLimit(true),
	    _HasUpperLimit(true)
	    {}

	unsigned int Number() const { return _NumberId; }
	const std::string& Name() const { return _Name; }
	double Value() const { return _Value; }
	double Error() const { return _Error; }
	
	void SetValue(double val) { _Value = val; }
	void SetError(double err) { _Error = err; }
	void SetLimits(double low, double up) {
	    assert(low != up);
	    if(low < up) {
		_LowerLimit = low;
		_UpperLimit = up;
	    }
	    else {
		_LowerLimit = up;
		_UpperLimit = low;
	    }
	    _HasLowerLimit = true;
	    _HasUpperLimit = true;
	}
	void SetLowerLimit(double low) {
	    _LowerLimit = low;
	    _UpperLimit = 0.;
	    _HasLowerLimit = true;
	    _HasUpperLimit = false;
	}
	void SetUpperLimit(double up) {
	    _LowerLimit = 0.;
	    _UpperLimit = up;
	    _HasLowerLimit = false;
	    _HasUpperLimit = true;
	}
	void RemoveLimits() {
	    _LowerLimit = 0.;
	    _UpperLimit = 0.;
	    _HasLowerLimit = false;
	    _HasUpperLimit = false;
	}

	bool HasLimits() const { return _HasLowerLimit || _HasUpperLimit; }
	bool HasLowerLimit() const { return _HasLowerLimit; }
	bool HasUpperLimit() const { return _HasUpperLimit; }
	double LowerLimit() const { return _LowerLimit; }
	double UpperLimit() const { return _UpperLimit; }
	
    };

} // namespace LQCDA

#endif	// FUNCTION_PARAMETER_HPP_
