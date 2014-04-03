/*
 * Fit.hpp
 *
 *  Created on: Feb 12, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FIT_HPP
#define FIT_HPP

 #include "Globals.hpp"
 #include "FitInterface.hpp"
 #include "CostFunction.hpp"
 #include "Minimizer.hpp"

 namespace LQCDA {

 	template<typename T>
 	class FitResult;

 	template<typename T>
 	class Fitter
 	{
 	public:
 		// Options
 		struct Options
 		{
 			Verbosity verbosity;
 			MinimizerID minimizer_type{MinimizerType::MIGRAD};
 			bool update_cost_fcn;
 		};

 	private:
 		// Typedefs
 		typedef ParametrizedScalarFunction<T> ScalarModel;

 		// XY data
 		const XYFitData<T>& _Data;
 		// Cost function
 		std::unique_ptr<Chi2CostFunction<T>> _CostFcn;
 		// Minimizer
 		std::unique_ptr<Minimizer<T>> _Minimizer;

 	public:
 		// Constructors
 		Fitter(const XYFitData<T>& data);
 		// Fitter(const XYData<T>& data, unsigned int npar);
 		Fitter(const XYFitData<T>& data, const std::vector<const ScalarModel *>& model);
 		// Destructor
 		virtual ~Fitter() noexcept = default;

 		// Update
 		void update();

 		// Fit method
 		FitResult<T> fit(
 			const std::vector<const ParametrizedScalarFunction<T> *>& model,
 			const std::vector<T>& x0,
 			const Options& opts = Options(),
 			const typename Minimizer<T>::Options& min_opts = typename Minimizer<T>::Options());
 		FitResult<T> fit(
 			const ParametrizedScalarFunction<T> & model,
 			const std::vector<T>& x0,
 			const Options& opts = Options(),
 			const typename Minimizer<T>::Options& min_opts = typename Minimizer<T>::Options());
 	};

 	template<typename T>
 	Fitter<T>::Fitter(const XYFitData<T>& data)
 	: _Data(data)
 	{}

 	template<typename T>
 	Fitter<T>::Fitter(
 		const XYFitData<T>& data,
 		const std::vector<const ParametrizedScalarFunction<T> *>& model)
 	: _Data(data)
 	, _CostFcn(new Chi2CostFunction<T>(data, model))
 	{}


 	template<typename T>
 	FitResult<T> Fitter<T>::fit(
 			const std::vector<const ParametrizedScalarFunction<T> *>& model,
 			const std::vector<T>& x0,
 			const Options& opts,
 			const typename Minimizer<T>::Options& min_opts)
 	{
 		utils::vostream vout(std::cout, opts.verbosity);
 		// Initialize
 		// Cost function
 		if(!_CostFcn)
 		{
 			vout(DEBUG) << "Creating chi2 cost function...\n";
 			_CostFcn = std::unique_ptr<Chi2CostFunction<T>>(new Chi2CostFunction<T>(_Data, model));
 		}
 		else
 		{
 			vout(DEBUG) << "Updating chi2 cost function model...\n";
 			_CostFcn->setModel(model);
 			if(opts.update_cost_fcn)
 			{
 				_CostFcn->requestUpdate();
 			}
 		}
 		// Minimizer
 		vout(DEBUG) << "Creating minimizer...\n";
 		_Minimizer = MinimizerFactory<T>::instance().create(opts.minimizer_type, min_opts);
 		// Initial parameters
 		std::vector<T> xinit(_CostFcn->xDim());
 		std::copy(x0.begin(), x0.end(), xinit.begin());
 		index_t xk{0}, di{0};
 		for(index_t k=0; k<_Data.xDim(); ++k)
 		if(!_Data.isXExact(k))
 		{
 			di = 0;
 			for(index_t i=0; i<_Data.nPoints(); ++i)
 			if(_Data.isFitPoint(i))
 			{
 				xinit[_CostFcn->nPar() + xk * _Data.nFitPoints() + di] = _Data.x(i, k);
 				di++;
 			}
 			xk++;
 		}

 		// Fit
 		FitResult<T> result;
 		result._nDOF = _CostFcn->nDOF();

 		auto min = _Minimizer->minimize(*_CostFcn, xinit);

 		result._Params = min.minimum;
 		result._Cost = min.final_cost;

 		return result;
 	}

 	template<typename T>
 	FitResult<T> Fitter<T>::fit(
 			const ParametrizedScalarFunction<T> & model,
 			const std::vector<T>& x0,
 			const Options& opts,
 			const typename Minimizer<T>::Options& min_opts)
 	{
 		std::vector<const ParametrizedScalarFunction<T> *> vmodel(1);
 		vmodel[0] = &model;
 		return fit(vmodel, x0, opts, min_opts);
 	}


 	template<typename T>
 	FitResult<T> Fit(
 		const XYFitData<T>& data,
 		const std::vector<const ParametrizedScalarFunction<T> *>& model,
 		const std::vector<T>& x0,
 		const typename Fitter<T>::Options& opts = Fitter<T>::Options(),
 		const typename Minimizer<T>::Options& min_opts = Minimizer<T>::Options())
 	{
 		return Fitter<T>(data).fit(model, x0, opts, min_opts);
 	}

 	template<typename T>
 	FitResult<T> Fit(
 		const XYFitData<T>& data,
 		const ParametrizedScalarFunction<T> & model,
 		const std::vector<T>& x0,
 		const typename Fitter<T>::Options& opts = typename Fitter<T>::Options(),
 		const typename Minimizer<T>::Options& min_opts = typename Minimizer<T>::Options())
 	{
 		Fitter<T> fitter(data);
 		return fitter.fit(model, x0, opts, min_opts);
 	}

  //   template<typename T>
 	// std::ostream& operator<< (std::ostream& out, const Fit<T>& res) {
 	// 	out << "\nFitFcn min value:\n"
 	// 		<< res.getFitFcnMinValue() << '\n';

 	// 	out	<< "\nFitted parameters:\n";
 	// 	for (auto p: res.getFittedParamValues())
 	// 		out << p << '\n';

 	// 	out	<< "\nFitted parameters errors:\n";
 	// 	for (auto p: res.getFittedParamErrors())
 	// 		out << p << '\n';

 	// 	out << '\n';

 	// 	return out;
 	// }

 }

#endif // FIT_HPP