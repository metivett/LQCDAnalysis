/*
 * Exceptions.hpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Thibaut Metivet
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

 #include <stdexcept>
 #include "Globals.hpp"

namespace LQCDA {

	#define ERROR(exception, message) throw(Exceptions::exception(message, SRC_LOC))

	#define DECL_EXC(name, base) \
	class name: public base\
	{\
	public:\
		explicit name(const std::string& msg, const std::string& src_loc);\
	};

	namespace Exceptions
	{

		DECL_EXC(IO, std::runtime_error);
		DECL_EXC(Implementation, std::logic_error)

	}

}

#endif // EXCEPTIONS_HPP