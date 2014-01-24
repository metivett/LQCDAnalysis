/*
 * Globals.hpp
 *
 *  Created on: Jan 28, 2014
 *      Author: Thibaut Metivet
 */

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

 #include <vector>
 #include <array>
 #include <complex>
 #include <string>
 #include <iostream>
 #include "Eigen/Core"

 #define SRC_LOC strFrom(__FUNC__) + " at " \
 + strFrom(__FILE__) + ":" \
 + strFrom(__LINE__)

 namespace LQCDA {

 	const int Dynamic = Eigen::Dynamic;

 	// Global enums
 	enum class Verbosity
 	{
 		Silent,
 		Normal,
 		Debug
 	};

 	template<typename T>
 	std::string strFrom(T x)
 	{
 		std::ostringstream oss;
 		oss << x;
 		return oss.str();
 	}

 	template<typename T>
 	T strTo(const std::string& str)
 	{
 		T res;
 		std::istringstream iss(str);
 		iss >> res;
 		return res;
 	}

 }

#endif // GLOBALS_HPP