/*
 * FitterImpl.hpp
 *
 *  Created on: Mar 13, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FITTER_IMPL_HPP
#define FITTER_IMPL_HPP

 #include "Fitter.hpp"

 namespace LQCDA {

 	template<typename T, int XDIM, int YDIM, int NPAR>
 	class FitterImpl
 	{
 	public:
 		// Typedefs
 		USING_PARAMETRIZED_FUNCTION_TYPEDEFS;
 		typedef ParametrizedFunction<T, XDIM, YDIM, NPAR> model_type;

 	public:
 		struct Options {
 			Options() {
 				minimizer_type = MIGRAD;
 				verbosity = SILENT;
 			}

 			MinimizerType minimizer_type;
 			Verbosity verbosity;
 		};

 		struct Result {
 			Result();
 		};

 	private:
 		// Verbosity
 		Verbosity _Verbosity;
 		// Cost Function
 		std::unique_ptr<cost_fcn_type> _CostFcn;
 		// Minimizer
 		std::unique_ptr<minimizer_type> _Minimizer;
 		// Data
 		unsigned int _Npts;
 		const vector<x_type> * _X_V;
 		const vector<y_type> * _Y_V;
 		std::unique_ptr<Vector<T, Dynamic>> * _X;
 		std::unique_ptr<Vector<T, Dynamic>> * _Y;
 		// Model
 		model_type * _Model;
 		std::unique_ptr<Vector<T, Dynamic>> * _MX;

 	public:
 		// Constructors/Destructor
 		Fitter(Verbosity verbosity = Verbosity::Silent)
 		: _Verbosity(verbosity)
 		, _Npts(0)
 		, _X_V(0)
 		, _Y_V(0)
 		, minimizer_type(verbosity)
 		{}

 		~Fitter() {}

 		// Fit methods
 		Fit<T, XDIM, YDIM, NPAR> fit(model_type * model, const vector<x_type> * x_v, const vector<y_type> * y_v);

 	private:
 		double eval_fit_fcn(params_type p);

 	};

 }

#endif // FITTER_IMPL_HPP