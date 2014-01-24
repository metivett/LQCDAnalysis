/*
 * FitResult.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_RESULT_HPP_
#define FIT_RESULT_HPP_

 #include "ParametrizedFunction.hpp"


 namespace LQCDA {

 	template<typename T, int XDIM, int YDIM, unsigned int NPAR>
 	class FitResult
 	{
 	private:
 		typedef ParametrizedFunction<Vector<T, XDIM>, Vector<T, YDIM>, NPAR> ModelT;

 		ModelT * _FittedModel;
 		std::vector<double> _ParamErrors;
 		double _FitFcnMinValue;

 	public:
 		FitResult(ModelT * fittedmodel, const std::vector<double>& errs, double min):
 		_FittedModel(fittedmodel), _ParamErrors(errs), _FitFcnMinValue(min)
 		{}

 		double getFittedParamValue(unsigned int i) const { return _FittedModel->getParamValue(i); }
 		const std::vector<double>& getFittedParamValues() const { return _FittedModel->getParamValues(); }
 		double getFittedParamError(unsigned int i) const { return _ParamErrors[i]; }
 		const std::vector<double>& getFittedParamErrors() const { return _ParamErrors; }

 		double getFitFcnMinValue() const { return _FitFcnMinValue; }
 	};

    template<typename T, int XDIM, int YDIM, unsigned int NPAR>
 	std::ostream& operator<< (std::ostream& out, const FitResult<T, XDIM, YDIM, NPAR>& res) {
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

} // namespace LQCDA

#endif	// FIT_RESULT_HPP_
