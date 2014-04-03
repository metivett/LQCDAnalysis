/*
 * Fitter.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FITTER_HPP_
#define FITTER_HPP_

 #include <memory>

 #include "Globals.hpp"
 #include "TypeTraits.hpp"
 #include "CostFunction.hpp"
 #include "Minimizer.hpp"


 namespace LQCDA {

 	template<typename T>
 	class Fitter
 	{
 	public:
 		struct Options {
 			Options() {
 				MinimizerType = MIGRAD;
 				verbosity = SILENT;
 			}

 			MinimizerType minimizer_type;
 			Verbosity verbosity;
 		};

 		struct Result {
 			Result();
 		};

 	private:
 		// State
 		struct FitterState
 		{
 			// Cost Function
 			std::unique_ptr<CostFunction<T>> _CostFcn;
 		};
 		// Verbosity
 		Verbosity _Verbosity;
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
 		// Constructors
 		Fitter(Verbosity verbosity = Verbosity::Silent)
 		: _Verbosity(verbosity)
 		, _Npts(0)
 		, _X_V(0)
 		, _Y_V(0)
 		, minimizer_type(verbosity)
 		{}
 		// Destructor
 		virtual ~Fitter() = default;

 		// Fit
 		virtual FitResult<T> fit(
 			const std::vector<const scalar_model_t *>& model,
 			const Vector<T>& init,
 			const Options& options = Options())
 			const =0;

 	private:
 		double eval_fit_fcn(params_type p);

 	};

 	template<
 	typename T, int XDIM, int YDIM, int NPAR,
 	template<typename, int, int, unsigned int> class FitPolicy,
 	template<typename> class Minimizer
 	>
 	Fit<T, XDIM, YDIM, NPAR> 
 	Fitter<T, XDIM, YDIM, NPAR, FitPolicy, Minimizer>::fit(model_type * model, const vector<x_type> * x_v, const vector<y_type> * y_v)
 	{
 		// Check sizes
 		if(x_v->size() != y_v->size())
 			ERROR(FIT, "Size of X != Size of Y");
 		_Npts = x_v->size();

 		// Initialize internal variables
 		_Model = model;
 		_X_V = x_v;
 		_Y_V = y_v;
 		_X = std::unique_ptr<Vector<T, Dynamic>>(new Vector<T, Dynamic>(_Npts * XDIM));
 		_Y = std::unique_ptr<Vector<T, Dynamic>>(new Vector<T, Dynamic>(_Npts * YDIM));
 		_MX = std::unique_ptr<Vector<T, Dynamic>>(new Vector<T, Dynamic>(_Npts * YDIM));
 		internal::fill<XDIM>(_X, x_v->begin(), x_v->end(), XDIM);
 		internal::fill<YDIM>(_Y, y_v->begin(), y_v->end(), YDIM);

 		// Get initial parameters
 		p_type init_p(model->getParamValues());

 		// Minimize
 		auto minimum = minimize(eval_fit_fcn, init_p);

 		// Return Fit
 		return Fit<T, XDIM, YDIM, NPAR>(model, minimum.UserParameters().Errors(), minimum.Fval());
 	}

 	template<
 	typename T, int XDIM, int YDIM, int NPAR,
 	template<typename, int, int, unsigned int> class FitPolicy,
 	template<typename> class Minimizer
 	>
 	double Fitter<T, XDIM, YDIM, NPAR, FitPolicy, Minimizer>::eval_fit_fcn(params_type p)
 	{
		// Set model parameters
 		_Model->setParValues(p);

 		// Compute model(x)
 		vector<y_type> mx(_Npts);
 		for(unsigned int i=0; i<_Npts; ++i)
 			mx[i] = (*_Model)(*_X_V[i]);
 		internal::fill<YDIM>(_MX, mx.begin(), mx.end(), YDIM);

 		// Compute and return FitFcn value
 		return evaluate(_X, _Y, _MX);
 	}

 	template<
 	template<typename, int, int, unsigned int> class FitPolicy, 
 	template<typename> class Minimizer,
 	typename T, int XDIM, int YDIM, unsigned int NPAR
 	>
 	Fit<T, XDIM, YDIM, NPAR> fit
 	( ParametrizedFunction<T, XDIM, YDIM, NPAR> * model
 		, const vector<typename ParametrizedFunction<T, XDIM, YDIM, NPAR>::x_type> * x_v
 		, const vector<typename ParametrizedFunction<T, XDIM, YDIM, NPAR>::y_type> * y_v)
 	{
 		Fitter<T, XDIM, YDIM, NPAR, FitPolicy, Minimizer> fitter;
 		return fitter.fit(model, x_v, y_v);
 	}

 	// template<typename T, int XDIM, int YDIM, unsigned int NPAR,
 	// template<typename, int, int, unsigned int> class FitPolicy, 
 	// template<typename, typename, typename...> class MinimizerPolicy,
 	// typename... MinimizerOpts
 	// >
 	// class Fitter: public FitPolicy<T, XDIM, YDIM, NPAR>
 	// {
 	// private:
 	// 	typedef FitPolicy<T, XDIM, YDIM, NPAR> FitFcn;
 	// 	typedef DataSet<T, XDIM, YDIM> DataT;
 	// 	typedef VParametrizedFunction<T, XDIM, YDIM, NPAR> ModelT;

 	// 	ModelT * _Model;

 	// public:
 	// 	Fitter(DataT * data, ModelT * model): FitFcn(data, model), _Model(model) {}

 	// 	FitResult<T, XDIM, YDIM, NPAR> fit() {
 	// 		// Get initial parameters
 	// 		std::vector<double> init_params(_Model->getParamValues().begin(), _Model->getParamValues().end());
 	// 		// Minimize
 	// 		auto minimum = Minimize<MinimizerPolicy, MinimizerOpts...>(this, init_params);
 	// 		return FitResult<T, XDIM, YDIM, NPAR>(_Model, minimum.UserParameters().Errors(), minimum.Fval());
 	// 	}
 	// };

 	// template<
 	// template<typename, int, int, unsigned int> class FitPolicy, 
 	// template<typename, typename, typename...> class MinimizerPolicy,
 	// typename... MinimizerOpts,
 	// typename T, int XDIM, int YDIM, unsigned int NPAR
 	// >
 	// Fitter<T, XDIM, YDIM, NPAR, FitPolicy, MinimizerPolicy, MinimizerOpts...> *
 	// MakeFitter(DataSet<T, XDIM, YDIM> * data, VParametrizedFunction<T, XDIM, YDIM, NPAR> * model)
 	// {
 	// 	return new Fitter<T, XDIM, YDIM, NPAR, FitPolicy, MinimizerPolicy, MinimizerOpts...>(data, model);
 	// }

 }

#endif	// FITTER_HPP_
