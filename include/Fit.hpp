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
 #include "IO.hpp"

 namespace LQCDA {

 	template<typename T>
 	class FitResult;

 	namespace internal {
	 	template<
	 	typename T, 
	 	template<typename> class COST,
	 	template<typename> class MINIMIZER
	 	>
 		class FitImpl
 		: public FitInterface
 		{
 		public:
	 		// Options
 			struct Options
 			{
 				Verbosity verbosity;
	 			// const MinimizerType& minimizer_type;
 				bool update_cost_fcn;

 				Options()
	 			// : minimizer_type(MIN::MIGRAD())
 				{}
 			};

 		private:
	 		// Typedefs
 			typedef ParametrizedScalarFunction<T> ScalarModel;

	 		// XY data
 			const XYDataInterface<T>& _Data;
	 		// Cost function
 			std::unique_ptr<COST<T>> _CostFcn;
 			// Options
 			Options _Opts;
	 		// Minimizer
 			std::unique_ptr<MINIMIZER<T>> _Minimizer;

 		public:
	 		// Constructors
 			FitImpl(const XYDataInterface<T>& data);
 			FitImpl(const XYDataInterface<T>& data, const std::vector<const ScalarModel *>& model);
	 		// Destructor
 			virtual ~FitImpl() = default;

	 		// Update
 			void update();

	 		// Accessors
 			void setOptions(const Options& opts);

	 		// Fit method
 			FitResult<T> fit(
 				const std::vector<const ParametrizedScalarFunction<T> *>& model,
 				const std::vector<T>& x0);
 			FitResult<T> fit(
 				const ParametrizedScalarFunction<T> & model,
 				const std::vector<T>& x0);
 		};

	 	template<
	 	typename T, 
	 	template<typename> class COST,
	 	template<typename> class MINIMIZER
	 	>
 		FitImpl<T, COST, MINIMIZER>::FitImpl(const XYDataInterface<T>& data)
 		: FitInterface(data.nPoints(), data.xDim(), data.yDim())
 		, _Data(data)
 		{}

		template<
	 	typename T, 
	 	template<typename> class COST,
	 	template<typename> class MINIMIZER
	 	> 		
	 	FitImpl<T, COST, MINIMIZER>::FitImpl(
 			const XYDataInterface<T>& data,
 			const std::vector<const ParametrizedScalarFunction<T> *>& model)
 		: FitInterface(data.nPoints(), data.xDim(), data.yDim())
 		, _Data(data)
 		, _CostFcn(new Chi2CostFunction<T>(data, model))
 		{}

 		template<
	 	typename T, 
	 	template<typename> class COST,
	 	template<typename> class MINIMIZER
	 	>
 		void FitImpl<T, COST, MINIMIZER>::setOptions(const Options& opts)
 		{
 			_Opts = opts;
 		}

	 	template<
	 	typename T, 
	 	template<typename> class COST,
	 	template<typename> class MINIMIZER
	 	>
 		FitResult<T> FitImpl<T, COST, MINIMIZER>::fit(
 			const std::vector<const ParametrizedScalarFunction<T> *>& model,
 			const std::vector<T>& x0)
 		{
 			utils::vostream vout(std::cout, _Opts.verbosity);
	 		// Initialize
	 		// Cost function
 			if(!_CostFcn)
 			{
 				vout(DEBUG) << "Creating cost function...\n";
 				_CostFcn = std::unique_ptr<COST<T>>(new COST<T>(_Data, *this, model));
 			}
 			else
 			{
 				vout(DEBUG) << "Updating cost function model...\n";
 				_CostFcn->setModel(model);
 				if(_Opts.update_cost_fcn)
 				{
 					_CostFcn->requestUpdate();
 				}
 			}
	 		// Minimizer
 			vout(DEBUG) << "Creating minimizer...\n";
	 		// _Minimizer = MIN::MinimizerFactory<T>::instance().create(MINIMIZER(), min_opts);
 			_Minimizer = std::unique_ptr<MINIMIZER<T>>(new MINIMIZER<T>);
	 		// Initial parameters
 			std::vector<T> xinit(_CostFcn->xDim());
 			std::copy(x0.begin(), x0.end(), xinit.begin());
 			index_t xk{0}, di{0};
 			for(index_t k=0; k<_Data.xDim(); ++k)
 				if(!this->isXExact(k))
 				{
 					di = 0;
 					for(index_t i=0; i<_Data.nPoints(); ++i)
 						if(this->isFitPoint(i))
 						{
 							xinit[_CostFcn->nPar() + xk * this->nFitPoints() + di] = _Data.x(i, k);
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

	 	template<
	 	typename T, 
	 	template<typename> class COST,
	 	template<typename> class MINIMIZER
	 	>
		FitResult<T> FitImpl<T, COST, MINIMIZER>::fit(
			const ParametrizedScalarFunction<T> & model,
			const std::vector<T>& x0)
		{
			std::vector<const ParametrizedScalarFunction<T> *> vmodel(1);
			vmodel[0] = &model;
			return fit(vmodel, x0);
		}

	 } // namespace internal

 	template<typename T, template<typename> class MINIMIZER>
 	using Chi2Fit = internal::FitImpl<T, Chi2CostFunction, MINIMIZER>;
	// template<template<typename> class MINIMIZER, typename T>
	// internal::FitImpl<T, Chi2CostFunction, MINIMIZER> Chi2Fit(
	// 	const XYDataInterface<T>& data)
 // 	{
	// 	return internal::FitImpl<T, Chi2CostFunction, MINIMIZER>(data);
	// }

 // 	template<template<typename> class MINIMIZER, typename T>
	// internal::FitImpl<T, Chi2CostFunction, MINIMIZER> Chi2Fit(
	// 	const XYDataInterfaceInterface<T>& data,
	//  	const std::vector<const ParametrizedScalarFunction<T> *>& model,
	//  	const std::vector<T>& x0,
	//  	const typename internal::FitImpl<T, COST, MINIMIZER>::Options& opts = internal::FitImpl<T, COST, MINIMIZER>::Options(),
	//  	const MIN::Options<MINIMIZER>& min_opts = MIN::Options<MIN::DEFAULT>())
	//  {
	//  	return internal::FitImpl<T, Chi2CostFunction>(data);
	//  	// return Fit<T>(data).fit(model, x0, opts, min_opts);
	//  }

 	// template<template<typename> class MINIMIZER, typename T>
	 // FitResult<T> Chi2Fit(
	 // 	const XYDataInterfaceInterface<T>& data,
	 // 	const ParametrizedScalarFunction<T> & model,
	 // 	const std::vector<T>& x0,
	 // 	const typename internal::FitImpl<T>::Options& opts = typename internal::FitImpl<T>::Options(),
	 // 	const MIN::Options<MINIMIZER>& min_opts = MIN::Options<MIN::DEFAULT>())
	 // {
	 // 	internal::FitImpl<T> fitter(data);
	 // 	return fitter.fit(model, x0, opts, min_opts);
	 // }

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