/*
 * io_utils.cpp
 *
 *  Created on: May 2, 2013
 *      Author: Thibaut Metivet
 */

#include "io_utils.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <functional>
#include <streambuf>

namespace LQCDA {

    class null_out_buf : public std::streambuf
    {
    public:
	virtual std::streamsize xsputn(const char* p, std::streamsize n) {
	    return n;
	}
	virtual int overflow(int c) {
	    return 1;
	}
    };

    null_out_buf nullbuf;
    std::ostream VerbosityFilteredOutStream::null_out(&nullbuf);

    std::ostream LQCDOut(std::cout.rdbuf());
    VerbosityFilteredOutStream LQCDDebug(std::cout, 1);

} // namespace LQCDA

//-----------------------------------------------------------
// Utility functions
//-----------------------------------------------------------
    void LQCDA::readArray(AsciiReader& reader, Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic>& output)
    {
	try {
	    std::string line;
	    reader.readLine(line);
	    std::istringstream isstr (line);
	    int i, j;
	    isstr >> i >> j;
	    output.resize(i, j);
	    for(int _i=0; _i<i; ++_i)
		for(int _j=0; _j<j; ++_j)
		{
		    reader.read(output(_i,_j));
		}
	}
	catch (const IOException& e) {

	}
    }
    void LQCDA::readVector(AsciiReader& reader, Eigen::Array<double, Eigen::Dynamic, 1>& output)
    {
	try {
	    int i;
	    reader.read(i);
	    output.resize(i);
	    for(int _i=0; _i<i; ++_i)
		reader.read(output(_i));
	}
	catch (const IOException& e) {

	}
    }
    void LQCDA::readVector(AsciiReader& reader, std::vector<double>& output)
    {
	try {
	    int i;
	    reader.read(i);
	    output.resize(i);
	    for(int _i=0; _i<i; ++_i)
		reader.read(output[_i]);
	}
	catch (const IOException& e) {

	}
    }
    bool LQCDA::getNextInstruction(AsciiReader& reader, std::string& output, IOFormat format)
    {
	std::string line;
	while((reader.readLine(line)))
	{
	    // trims white space from the beginning of the line
	    line.erase(line.begin(), find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(isspace))));
	    switch (format)
	    {
	    case IOFormat::LATAN_1:
		if(line[0]=='#')
		{
		    output = line;
		    return true;
		}
		break;
	    }
	}
	return false;
    }
