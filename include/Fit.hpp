/*
 * Fit.hpp
 *
 *  Created on: Feb 12, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FIT_HPP
#define FIT_HPP

 #include "ParametrizedFunction.hpp"

 namespace LQCDA {

 	template<typename T, int XDIM, int YDIM, unsigned int NPAR>
 	class Fit
 	{
 	private:
 		// Typedefs
 		typedef ParametrizedFunction<T, XDIM, YDIM, NPAR> model_type;

 		const model_type * _FittedModel;
 		std::vector<double> _ParamErrors;
 		double _FitFcnMinValue;

 	public:
 		Fit(const model_type * fittedmodel, const std::vector<double>& errs, double min)
 		: _FittedModel(fittedmodel)
 		, _ParamErrors(errs)
 		, _FitFcnMinValue(min)
 		{}

 		double getFittedParamValue(unsigned int i) const { return _FittedModel->getParamValue(i); }
 		const std::vector<double>& getFittedParamValues() const { return _FittedModel->getParamValues(); }
 		double getFittedParamError(unsigned int i) const { return _ParamErrors[i]; }
 		const std::vector<double>& getFittedParamErrors() const { return _ParamErrors; }

 		double getFitFcnMinValue() const { return _FitFcnMinValue; }
 	};

    template<typename T, int XDIM, int YDIM, unsigned int NPAR>
 	std::ostream& operator<< (std::ostream& out, const Fit<T, XDIM, YDIM, NPAR>& res) {
 		out << "\nFitFcn min value:\n"
 			<< res.getFitFcnMinValue() << '\n';

 		out	<< "\nFitted parameters:\n";
 		for (auto p: res.getFittedParamValues())
 			out << p << '\n';

 		out	<< "\nFitted parameters errors:\n";
 		for (auto p: res.getFittedParamErrors())
 			out << p << '\n';

 		out << '\n';

 		return out;
 	}

 }

#endif // FIT_HPP