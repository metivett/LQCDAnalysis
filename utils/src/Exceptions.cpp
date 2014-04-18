/*
 * Exceptions.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: Thibaut Metivet
 */

 #include "Exceptions.hpp"

 using namespace std;
 using namespace utils;
 using namespace Exceptions;

 #define DEF_EXC(name, init) \
 name::name(const std::string& msg, const std::string& src_loc) \
 : init \
 {}

 // Logic exceptions
 DEF_EXC(LOGIC, logic_error(msg + " (" + src_loc + ") "))
 DEF_EXC(IMPLEMENTATION, LOGIC("implementation error: " + msg, src_loc))
 DEF_EXC(SIZE, LOGIC("size error: " + msg, src_loc))
 // Runtime exceptions
 DEF_EXC(RUNTIME, runtime_error(msg + " (" + src_loc + ") "))
 DEF_EXC(IO, RUNTIME("IO error: " + msg, src_loc))
 DEF_EXC(MEMORY, RUNTIME("memory error: " + msg, src_loc))
 DEF_EXC(NULLPTR, RUNTIME("null pointer error: " + msg, src_loc))
