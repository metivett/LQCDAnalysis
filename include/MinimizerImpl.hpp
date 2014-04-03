/*
 * MinimizerImpl.hpp
 *
 *  Created on: Mar 12, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MINIMIZER_IMPL_HPP
#define MINIMIZER_IMPL_HPP

 #include "Minimizer.hpp"

 namespace LQCDA {
 	namespace internal {

 		template<MinimizerType MINIMIZER, typename T>
 		class MinimizerImpl
 		{
 		public:
 			struct OptionsImpl {
 				OptionsImpl() { 
 					verbosity = SILENT; 
 				}

 				Verbosity verbosity;
 			};

 		public:
 			virtual ~MinimizerImpl() = default;

 			virtual Minimizer::Result * minimize(
 			const ScalarFunction<T>& F, 
 			const std::vector<T>& x0, 
 			const Options<MINIMIZER>& options = Options<MINIMIZER>()) =0;
 		};

 	}
 }

#endif // MINIMIZER_IMPL_HPP