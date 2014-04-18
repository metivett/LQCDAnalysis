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

 namespace LQCDA {
 	namespace MIN {

 		template<template<typename> class MINTYPE>
 		class Options;
 		template<typename T>
 		class Minimizer;

 		template<typename T>
 		using DEFAULT = Minimizer<T>;

 		template<typename T>
 		class Minimizer
 		{
 		public:
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

 			// Options
 			virtual Options<DEFAULT>& options() =0;

 			// Minimize
 			virtual Result minimize(
 				const ScalarFunction<scalar_type>& F, 
 				const std::vector<scalar_type>& x0) =0;
 		};


 		template<>
 		class Options<DEFAULT>
 		{
 		public:
 			Verbosity verbosity;

 		public:
 			Options() {
 				verbosity = NORMAL;
 			}

 			virtual ~Options() noexcept = default;
 		};

 		template<typename T>
 		std::ostream& operator<< (std::ostream& out, const typename Minimizer<T>::Result& result)
 		{
 			out << "Final cost = " << result.final_cost << std::endl;
 			return out;
 		}
	}
 }

#endif // MINIMIZER_HPP