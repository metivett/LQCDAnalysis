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
 #include <memory>


/******************************************************************************
 *                                 Utilities                                  *
 ******************************************************************************/

 #define MAKE_STRING(a) std::string(#a)

 namespace utils {

 	namespace {
 		template<class T>
 		void print(std::ostringstream& os, T&& t)
 		{
 			os << std::forward<T>(t);
 		}
 		template<class T, class... Ts>
 		void print(std::ostringstream& os, T&& t, Ts&&... ts)
 		{
 			os << std::forward<T>(t);
 			print(os, std::forward<Ts>(ts)...);
 		}
 	}

 	template<typename... Ts>
 	std::string strFrom(Ts... x)
 	{
 		std::ostringstream oss;
 		// oss << x;
 		print(oss, x...);
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

 	template<typename T, typename ...Args>
 	std::unique_ptr<T> make_unique( Args&& ...args )
 	{
 		return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
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