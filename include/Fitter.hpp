/*
 * Fitter.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FITTER_HPP_
#define FITTER_HPP_

 #include "Globals.hpp"
 #include "Fit.hpp"
 #include "FitPolicy.hpp"
 #include "Minimizer.hpp"
 #include "Minuit2Minimizer.hpp"

 namespace LQCDA {

 	template<
 	typename T, int XDIM, int YDIM, int NPAR,
 	template<typename, int, int, unsigned int> class FitPolicy,
 	template<typename> class Minimizer
 	>
 	class Fitter
 	: public FitPolicy<T, XDIM, YDIM, NPAR>
 	, public Minimizer<FitPolicy<T, XDIM, YDIM, NPAR>>
 	{
 	public:
 		// Typedefs
 		typedef ParametrizedFunction<T, XDIM, YDIM, NPAR> model_type;
 		typedef Minimizer<FitPolicy<T, XDIM, YDIM, NPAR>> minimizer_type;

 	private:
 		// Verbosity
 		Verbosity _Verbosity;
 		// Data
 		vector<x_type> _X;
 		vector<y_type> _Y;
 		// Model

 	public:
 		// Constructors/Destructor
 		Fitter(Verbosity verbosity = Verbosity::Silent)
 		: _Verbosity(verbosity)
 		, minimizer_type(verbosity)
 		{}

 		~Fitter() {}

 		// Fit methods
 		template<typename Iterator>
 		Fit<T, XDIM, YDIM, NPAR> fit(model_type * model, Iterator x_it, Iterator y_it, unsigned int npts);
 		Fit<T, XDIM, YDIM, NPAR> fit(model_type * model, const vector<x_type>& x_v, const vector<y_type>& y_v) 
 		{
 			if(x_v.size() != y_v.size())
 				ERROR();
 			return fit(model, x_v.begin(), y_v.begin(), x_v.size());
 		}

 	};

 	template<
 	typename T, int XDIM, int YDIM, int NPAR,
 	template<typename, int, int, unsigned int> class FitPolicy,
 	template<typename> class Minimizer
 	>
 	Fit<T, XDIM, YDIM, NPAR> 
 	Fitter<T, XDIM, YDIM, NPAR, FitPolicy, Minimizer>::fit(model_type * model, Iterator x_it, Iterator y_it, unsigned int npts)
 	{
 		// Get initial parameters
 		std::vector<double> init_params(model->getParamValues());
 		// Set x and y
 		init(model, x_it, y_it, npts);
 		// Minimize
 		auto minimum = minimize(this, init_params);
 		// Return Fit
 		return Fit<T, XDIM, YDIM, NPAR>(model, minimum.UserParameters().Errors(), minimum.Fval());
 	}

 	template<
 	template<typename, int, int, unsigned int> class FitPolicy, 
 	template<typename> class Minimizer,
 	typename T, int XDIM, int YDIM, unsigned int NPAR,
 	typename Iterator
 	>
 	Fit<T, XDIM, YDIM, NPAR> fit(ParametrizedFunction<T, XDIM, YDIM, NPAR> * model, Iterator data_begin, Iterator data_end)
 	{
 		Fitter<T, XDIM, YDIM, NPAR, FitPolicy, Minimizer> fitter;
 		return fitter.fit(model, data_begin, data_end);
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
