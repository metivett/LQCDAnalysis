/*
 * Minimizer.hpp
 *
 *  Created on: Feb 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MINIMIZER_HPP
#define MINIMIZER_HPP

 #include "Function.hpp"
 #include "Minuit2/FunctionMinimum.h"

 namespace LQCDA {

 	template<typename Fcn, typename Args,
 		template<typename, typename, typename...> class MinimizerPolicy,
 	typename... MinimizerOpts>
 	class Minimizer: public MinimizerPolicy<Fcn, Args, MinimizerOpts...>
 	{};


 	template<
 	template<typename, typename, typename...> class MinimizerPolicy,
 	typename... MinimizerOpts,
 	typename Fcn, typename Args>
 	ROOT::Minuit2::FunctionMinimum Minimize(const Fcn* f, const Args& initargs)
 	{
 		return Minimizer<Fcn, Args, MinimizerPolicy, MinimizerOpts...>::minimize(f, initargs);
 	}

 }

#endif // MINIMIZER_HPP