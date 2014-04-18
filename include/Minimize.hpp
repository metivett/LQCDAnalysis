/*
 * Minimize.hpp
 *
 *  Created on: Apr 08, 2014
 *      Author: Thibaut Metivet
 */

 #ifndef MINIMIZE_HPP
 #define MINIMIZE_HPP

 #include "Minimizer.hpp"
 #include "MinimizerFactory.hpp"

 namespace LQCDA {
 	namespace MIN {

 		template<template<typename> class MINIMIZER, typename T>
 		typename Minimizer<T>::Result Minimize(
 			const ScalarFunction<T>& F, 
 			const std::vector<T>& x0,
 			const Options<MINIMIZER>& opts)
 		{
 			return MINIMIZER<T>(opts).minimize(F, x0);
 		}

 		template<typename T, template<typename> class MINIMIZER>
 		std::unique_ptr<MINIMIZER<T>> MakeMinimizer(const Options<MINIMIZER>& opts)
 		{
 			return std::unique_ptr<MINIMIZER<T>>(new MINIMIZER<T>(opts));
 		}
 		template<typename T, template<typename> class MINIMIZER>
 		std::unique_ptr<MINIMIZER<T>> MakeMinimizer()
 		{
 			return std::unique_ptr<MINIMIZER<T>>(new MINIMIZER<T>());
 		}

 		template<typename T>
 		std::unique_ptr<Minimizer<T>> MakeMinimizer(const MinimizerType& id)
 		{
 			return MinimizerFactory<T>::instance().create(id);
 		}

 	}
 }

#endif // MINIMIZE_HPP