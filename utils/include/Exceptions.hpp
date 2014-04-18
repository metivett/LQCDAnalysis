/*
 * Exceptions.hpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Thibaut Metivet
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

 #include <stdexcept>

 #include "Utilities.hpp"

 #define ERROR(exception, message) throw(Exceptions::exception(message, SRC_LOC))
 #define WARNING(msg) \
 std::cerr << "warning: " << msg \
 	<< " (" << SRC_LOC << ") " << std::endl;

 #define DECL_EXC(name, base) \
 class name: public base\
 {\
 	public:\
 	explicit name(const std::string& msg, const std::string& src_loc);\
 };

 namespace Exceptions
 {
	// Logic exceptions
 	DECL_EXC(LOGIC, std::logic_error)
 	DECL_EXC(IMPLEMENTATION, LOGIC)
 	DECL_EXC(SIZE, LOGIC)
	// Runtime exceptions
 	DECL_EXC(RUNTIME, std::runtime_error)
 	DECL_EXC(IO, RUNTIME)
 	DECL_EXC(MEMORY, RUNTIME)
 	DECL_EXC(NULLPTR, RUNTIME)
 }


#endif // EXCEPTIONS_HPP