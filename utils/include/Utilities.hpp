/*
 * Utilities.hpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Thibaut Metivet
 */

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

 #include <string>
 #include <sstream>
 #include <iostream>


/******************************************************************************
 *                                 Utilities                                  *
 ******************************************************************************/

 #define MAKE_STRING(a) std::string(#a)

 namespace utils {

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

 #define SRC_LOC \
 utils::strFrom(__func__) + " at " \
 + utils::strFrom(__FILE__) + ":" \
 + utils::strFrom(__LINE__)

 /******************************************************************************
 *                                 Assertion                                  *
 ******************************************************************************/
#ifdef LQCDA_NO_DEBUG
	#define ASSERT(x)
#else
	namespace utils {
		inline void assert_fail(const std::string& assertion, const std::string& src_loc)
		{
			std::cerr << "Assertion \"" + assertion + "\" failed (" + src_loc + ") " << std::endl;
		}
	}
	#define ASSERT(x) \
		do { \
			if(!(x)) \
				utils::assert_fail(std::string(#x), SRC_LOC); \
		} while(0)
#endif

#endif // UTILITIES_HPP