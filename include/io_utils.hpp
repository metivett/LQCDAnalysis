/*
 * io_utils.hpp
 *
 *  Created on: May 2, 2013
 *      Author: Thibaut Metivet
 */

#ifndef IO_UTILS_HPP_
#define IO_UTILS_HPP_

#include "io.hpp"
#include "Eigen/Dense"

namespace LQCDA {

    /*
     * class VerbosityFilteredOutStream
     * Print depending on verbosity and level
     * Verbosity can be 0, 1, 2, 3 with 0 skipping everything
     */
    class VerbosityFilteredOutStream
    {
    public:
	VerbosityFilteredOutStream(std::ostream& os, unsigned int v_lev)
	    : out(os.rdbuf()), level(v_lev) {}

	std::ostream& operator() (unsigned int v) {
	    if(v<=level) {
		return out;
	    }
	    else {
		return null_out;
	    }
	}
    private:
	std::ostream out;
	static std::ostream null_out;
	
	unsigned int level;		// verbosity level
    };

/*
 * LQCDA output streams
 */
    extern std::ostream LQCDOut;   // standard output
    extern VerbosityFilteredOutStream LQCDDebug; // debugging output
    
/*
 * Enumeration to deal with several IO formats (or versions)
 */
    enum IOFormat
    {
	LATAN_1
    };


// Utility functions to parse input data
/**
 * Reads a "matrix" of data, separated by spaces or line returns.
 * The first "line" should provide 2 integers representing the dimensions of the array
 */
    void readArray(AsciiReader& reader, Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic>& output);
/**
 * Reads a "vector" of data of type T, separated by spaces or line returns.
 * The first "line" should provide 1 integer representing the dimension of the vector array
 */
    void readVector(AsciiReader& reader, Eigen::Array<double, Eigen::Dynamic, 1>& output);
    void readVector(AsciiReader& reader, std::vector<double>& output);
/**
 * Reads the next instruction, introduced with character specified in format f
 * Returns a boolean informing whether instruction was found.
 */
    bool getNextInstruction(AsciiReader& reader, std::string& output, IOFormat f);


} // namespace LQCDA

#endif /* IO_UTILS_HPP_ */
