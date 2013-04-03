/*
 * io.hpp
 *
 *  Created on: Feb 14, 2013
 *      Author: Thibaut Metivet
 */

#ifndef IO_HPP_
#define IO_HPP_

#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include "Eigen/Dense"

namespace LQCDA {

/**
 * Enumeration to deal with several IO formats (or versions)
 */
enum IOFormat
{
	LATAN_1
};

/**
 * virtual class (interface) AsciiReader
 * Interface used to read an ascii-formatted input.
 */
class AsciiReader
{
public:
	AsciiReader ();	// Empty constructor
	virtual ~AsciiReader () {}	// Destructor

	// Different bindings for the 'read' operation
    virtual void read(std::string& result);
    virtual void read(char& result);
    virtual void read(int& result);
    virtual void read(unsigned int& result);
    virtual void read(short int& result);
    virtual void read(unsigned short int& result);
    virtual void read(long int& result);
    virtual void read(unsigned long int& result);
    virtual void read(long long int& result);
    virtual void read(float& result);
    virtual void read(double& result);
    virtual void read(bool& result);

    virtual bool readLine (std::string& output);

protected:
    template<typename T>
    void readPrimitive (T& output);

    virtual std::istream& getIstream () = 0;
};

AsciiReader& operator>>(AsciiReader& txt, std::string& input);
AsciiReader& operator>>(AsciiReader& txt, char& input);
AsciiReader& operator>>(AsciiReader& txt, int& input);
AsciiReader& operator>>(AsciiReader& txt, unsigned int& input);
AsciiReader& operator>>(AsciiReader& txt, short int& input);
AsciiReader& operator>>(AsciiReader& txt, unsigned short int& input);
AsciiReader& operator>>(AsciiReader& txt, long int& input);
AsciiReader& operator>>(AsciiReader& txt, unsigned long int& input);
AsciiReader& operator>>(AsciiReader& txt, long long int& input);
AsciiReader& operator>>(AsciiReader& txt, float& input);
AsciiReader& operator>>(AsciiReader& txt, double& input);
AsciiReader& operator>>(AsciiReader& txt, bool& input);


/**
 * class AsciiFileReader
 * Used to read an ascii-formatted file.
 */
class AsciiFileReader : public AsciiReader
{
public:
	AsciiFileReader ();	// Empty constructor
	explicit AsciiFileReader (const std::string& filename);	// Constructs and reads from file
	~AsciiFileReader ();	// Destructor

	void open(const std::string& filename);	// Opens and reads an ascii file.
	void open(std::istream& is);	// Opens and reads an ascii file.

	// Queries whether the file is open
	/*!
	 \return true if the file is open; false otherwise.
	 */
	bool is_open();

	//! Closes the last file opened
	void close();

	//! Return the entire contents of the Reader as a stream
	void print(std::ostream& is);

protected:
	std::ifstream& getIstream () { return ifs; }	// Returns internal filestream
private:
	std::ifstream ifs;
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
/**
 * Reads the next instruction, introduced with character specified in format f
 * Returns a boolean informing whether instruction was found.
 */
bool getNextInstruction(AsciiReader& reader, std::string& output, IOFormat f);



}	// namespace LQCDA


#endif /* IO_HPP_ */
