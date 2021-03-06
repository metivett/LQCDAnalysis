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
#include <iostream>

namespace LQCDA {

/**
 * virtual class (interface) Reader
 * Interface used to read an input.
 */
    class Reader
    {
    public:
	Reader ();	// Empty constructor
	virtual ~Reader () {}	// Destructor

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
	virtual bool skipLine();

    protected:
	template<typename T>
	void readPrimitive (T& output);

	virtual std::istream& getIstream () = 0;
    };

    Reader& operator>>(Reader& txt, std::string& input);
    Reader& operator>>(Reader& txt, char& input);
    Reader& operator>>(Reader& txt, int& input);
    Reader& operator>>(Reader& txt, unsigned int& input);
    Reader& operator>>(Reader& txt, short int& input);
    Reader& operator>>(Reader& txt, unsigned short int& input);
    Reader& operator>>(Reader& txt, long int& input);
    Reader& operator>>(Reader& txt, unsigned long int& input);
    Reader& operator>>(Reader& txt, long long int& input);
    Reader& operator>>(Reader& txt, float& input);
    Reader& operator>>(Reader& txt, double& input);
    Reader& operator>>(Reader& txt, bool& input);
    
/**
 * Base class AsciiReader
 * Used to read an ascii-formatted input.
 */
    class AsciiReader : public Reader
    {
    public:
	AsciiReader ();	// Empty constructor (reads from cin by default)
	explicit AsciiReader (std::istream& in);	// Constructs and reads from stream
	virtual ~AsciiReader () {}	// Destructor

    protected:
	virtual std::istream& getIstream () { return is; }	// Returns internal inputstream

    private:
	std::streambuf* sbuf;
	std::istream is;
    };

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
	void print(std::ostream& os);

    protected:
	std::ifstream& getIstream () { return ifs; }	// Returns internal filestream
    private:
	std::ifstream ifs;
    };


      
/**
 * virtual class (interface) Writer
 * Interface used to write to an input.
 */
    class Writer
    {
    public:
	Writer ();	// Empty constructor
	virtual ~Writer () {}	// Destructor

	// Different bindings for the 'read' operation
	virtual void write(std::string result);
	virtual void write(const char* result);
	virtual void write(char result);
	virtual void write(int result);
	virtual void write(unsigned int result);
	virtual void write(short int result);
	virtual void write(unsigned short int result);
	virtual void write(long int result);
	virtual void write(unsigned long int result);
	virtual void write(long long int result);
	virtual void write(float result);
	virtual void write(double result);
	virtual void write(bool result);

	virtual bool writeLine (std::string output);

    protected:
	template<typename T>
	void writePrimitive (T output);

	virtual std::ostream& getOstream () = 0;
    };

    Writer& operator<<(Writer& txt, std::string input);
    Writer& operator<<(Writer& txt, const char* input);
    Writer& operator<<(Writer& txt, char input);
    Writer& operator<<(Writer& txt, int input);
    Writer& operator<<(Writer& txt, unsigned int input);
    Writer& operator<<(Writer& txt, short int input);
    Writer& operator<<(Writer& txt, unsigned short int input);
    Writer& operator<<(Writer& txt, long int input);
    Writer& operator<<(Writer& txt, unsigned long int input);
    Writer& operator<<(Writer& txt, long long int input);
    Writer& operator<<(Writer& txt, float input);
    Writer& operator<<(Writer& txt, double input);
    Writer& operator<<(Writer& txt, bool input);

    
/**
 * Base class (interface) AsciiWriter
 * Used to write an ascii-formatted input.
 */
    class AsciiWriter : public Writer
    {
    public:
	AsciiWriter ();				  // Empty constructor
	explicit AsciiWriter (std::ostream& out); // Constructs and writes to stream
	virtual ~AsciiWriter () {}		  // Destructor

    protected:
	virtual std::ostream& getOstream () { return os; } // Returns internal outputstream

    private:
	std::streambuf* sbuf;
	std::ostream os;
    };

    
/**
 * class AsciiFileWriter
 * Used to write an ascii-formatted file.
 */
    class AsciiFileWriter : public AsciiWriter
    {
    public:
	AsciiFileWriter ();	// Empty constructor
	explicit AsciiFileWriter (const std::string& filename);	// Constructs and reads from file
	~AsciiFileWriter ();	// Destructor

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
	void print(std::ostream& os);

    protected:
	std::ofstream& getOstream () { return ofs; }	// Returns internal filestream
    private:
	std::ofstream ofs;
    };


}	// namespace LQCDA


#endif /* IO_HPP_ */
