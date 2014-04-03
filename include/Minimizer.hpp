/*
 * Minimizer.hpp
 *
 *  Created on: Feb 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MINIMIZER_HPP
#define MINIMIZER_HPP

 #include "Globals.hpp"
 #include "Function.hpp"
 #include "MinimizerFactory.hpp"

 namespace LQCDA {

 	template<typename T>
 	class Minimizer
 	{
 	public:
 		class Options
 		{
 		public:
 			Verbosity verbosity;

 		public:
 			Options(MinimizerID type = MinimizerType::DEFAULT) {
 				verbosity = NORMAL;
 			}

 			virtual ~Options() noexcept = default;

 		};

 		struct Result
 		{
 			std::vector<double> minimum;
 			double final_cost;
 			bool is_valid;
 		};

 		// Typedefs
 		typedef T scalar_type;

 	public:
 		// Destructor
 		virtual ~Minimizer() noexcept = default;

 		// Type id
 		static MinimizerID ID;

 		// Minimize
 		virtual Result minimize(
 			const ScalarFunction<T>& F, 
 			const std::vector<T>& x0) =0;
 	};

 	template<typename T>
 	MinimizerID Minimizer<T>::ID = MinimizerType::DEFAULT;

 	template<typename T>
 	typename Minimizer<T>::Result Minimize(
 		const ScalarFunction<T>& F, 
 		const std::vector<T>& x0,
 		const MinimizerID & Type,
 		const typename Minimizer<T>::Options& opts = typename Minimizer<T>::Options())
 	{
 		auto minimizer = MinimizerFactory<T>::instance().create(Type, opts);
 		return minimizer->minimize(F, x0);
 	}

 	template<typename T>
 	std::ostream& operator<< (std::ostream& out, const typename Minimizer<T>::Result& result)
 	{
 		out << "Final cost = " << result.final_cost << std::endl;
 		return out;
 	}

 	// class Minimizer
 	// { 	
 	// public:
 	// 	struct Result {
 	// 		Result();

 	// 		std::vector<double> minimum;
 	// 		double final_cost;
 	// 		bool is_valid;
 	// 	};

 	// public:
 	// 	// Minimize methods
 	// 	template<MinimizerType MINIMIZER, typename T>
 	// 	static Result * minimize(
 	// 		const ScalarFunction<T>& F, 
 	// 		const T * x0, 
 	// 		const Options<MINIMIZER>& options = Options<MINIMIZER>());
 	// 	template<MinimizerType MINIMIZER, typename T>
 	// 	static Result * minimize(
 	// 		const ScalarFunction<T>& F, 
 	// 		const std::vector<T>& x0, 
 	// 		const Options<MINIMIZER>& options = Options<MINIMIZER>());
 	// };

 	// template<typename T>
 	// class MnMigradMinimizer;

 	// template<MinimizerType MINIMIZER, typename T>
 	// Minimizer::Result * Minimizer::minimize(
 	// 	const ScalarFunction<T>& F, 
 	// 	const T * x0, 
 	// 	const Options<MINIMIZER>& options)
 	// {
 	// 	if(MINIMIZER == MIGRAD) {
 	// 		MnMigradMinimizer<T> minimizer;
 	// 		return minimizer.minimize(F, std::vector<double>(x0, x0 + F.xDim()), options);
 	// 	}
 	// }
 	// template<MinimizerType MINIMIZER, typename T>
 	// Minimizer::Result * Minimizer::minimize(
 	// 	const ScalarFunction<T>& F, 
 	// 	const std::vector<T>& x0, 
 	// 	const Options<MINIMIZER>& options)
 	// {
 	// 	if(MINIMIZER == MIGRAD) {
 	// 		MnMigradMinimizer<T> minimizer;
 	// 		return minimizer.minimize(F, x0, options);
 	// 	}
 	// }

 	// template<MinimizerType MINIMIZER, typename T>
 	// Minimizer::Result * Minimize(
 	// 	const ScalarFunction<T>& F, 
 	// 	const T * x0, 
 	// 	const Options<MINIMIZER>& options = Options<MINIMIZER>()) 
 	// {
 	// 	return Minimizer::minimize(F, x0, options);
 	// }
 	// template<MinimizerType MINIMIZER, typename T>
 	// Minimizer::Result * Minimize(
 	// 	const ScalarFunction<T>& F, 
 	// 	const std::vector<T>& x0, 
 	// 	const Options<MINIMIZER>& options = Options<MINIMIZER>()) 
 	// {
 	// 	return Minimizer::minimize(F, x0, options);
 	// }

 	// std::ostream& operator<< (std::ostream& out, const Minimizer::Result& result);

 }

#endif // MINIMIZER_HPP