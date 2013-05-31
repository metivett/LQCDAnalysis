/*
 * exceptions.hpp
 *
 *  Created on: Feb 11, 2013
 *      Author: Thibaut Metivet
 */

#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <string>

namespace LQCDA {

// TODO: add virtual print member function to print in cerr relevant information

    class LQCDA_Exception
    {
    public:
	LQCDA_Exception () {};
	LQCDA_Exception (const char * cstr)
	    : className(cstr)
	    {}
	LQCDA_Exception (const std::string str)
	    : className(str)
	    {}

	std::string className;
    };

    class OutOfRange : public LQCDA_Exception
    {
    public:
	OutOfRange (int n)
	    : LQCDA_Exception(), N(n) {}
	OutOfRange (const char * cstr, int n)
	    : LQCDA_Exception(cstr), N(n) {}
	OutOfRange (std::string str, int n)
	    : LQCDA_Exception(str), N(n) {}

	int N;
    };

    class DataException : public LQCDA_Exception
    {
    public:
	DataException(const char* m)
	    : LQCDA_Exception(), msg(m) {}

	std::string msg;
    };

    class IOException : public LQCDA_Exception
    {
    public:
	IOException (const char * classname, const char * file)
	    : LQCDA_Exception(classname), filename(file)
	    {}

	std::string filename;
    };

    class MinimizationException : public LQCDA_Exception
    {
    public:
	MinimizationException()
	    : LQCDA_Exception("Minimization") {}
    };

    class ResamplingException : public LQCDA_Exception
    {
    public:
	ResamplingException(const char* msg)
	    : LQCDA_Exception(), _msg(msg) {}
	std::string _msg;
    };

    class BadPointException
    {
    private:
	int _Index;
	
    public:
	BadPointException() : _Index(-1) {}
	BadPointException(int index) : _Index(index) {}

	int Index() { return _Index; }
    };

} // namespace LQCDA

#endif /* EXCEPTIONS_HPP_ */
