/*
 * io.cpp
 *
 *  Created on: Feb 14, 2013
 *      Author: Thibaut Metivet
 */

#include "io.hpp"
#include "exceptions.hpp"
#include <limits>

namespace LQCDA
{

//-----------------------------------------------------------
// Reader
//-----------------------------------------------------------
    Reader::Reader () {}
    
// String reader
    void Reader::read(std::string& result)
    {
	getIstream() >> result;
    }

// Readers
    void Reader::read(char& result)
    {
	readPrimitive<char> (result);
    }
    void Reader::read(int& result)
    {
	readPrimitive<int> (result);
    }
    void Reader::read(unsigned int& result)
    {
	readPrimitive<unsigned int> (result);
    }
    void Reader::read(short int& result)
    {
	readPrimitive<short int> (result);
    }
    void Reader::read(unsigned short int& result)
    {
	readPrimitive<unsigned short int> (result);
    }
    void Reader::read(long int& result)
    {
	readPrimitive<long int> (result);
    }
    void Reader::read(unsigned long int& result)
    {
	readPrimitive<unsigned long int> (result);
    }
    void Reader::read(long long int& result)
    {
	readPrimitive<long long int> (result);
    }
    void Reader::read(float& result)
    {
	readPrimitive<float> (result);
    }
    void Reader::read(double& result)
    {
	readPrimitive<double> (result);
    }
    void Reader::read(bool& result)
    {
	readPrimitive<bool> (result);
    }
    bool Reader::readLine(std::string& output)
    {
	std::istream& is = getIstream();
	getline(is, output);
	return !is.fail();
    }

    bool Reader::skipLine()
    {
	std::istream& is = getIstream();
	is.ignore(std::numeric_limits<int>::max(), '\n');
	return !is.fail();
    }

    template<typename T>
    void Reader::readPrimitive(T& output)
    {
	getIstream() >> output;
    }

    Reader& operator>>(Reader& txt, std::string& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, char& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, int& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, unsigned int& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, short int& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, unsigned short int& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, long int& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, unsigned long int& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, long long int& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, float& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, double& input) {
	txt.read(input);
	return txt;
    }
    Reader& operator>>(Reader& txt, bool& input) {
	txt.read(input);
	return txt;
    }

    
//-----------------------------------------------------------
// AsciiReader
//-----------------------------------------------------------
    AsciiReader::AsciiReader ()
	: sbuf(std::cin.rdbuf()), is(sbuf)
    {}

    AsciiReader::AsciiReader (std::istream& in)
	: sbuf(in.rdbuf()), is(sbuf)
    {}

//-----------------------------------------------------------
// AsciiFileReader
//-----------------------------------------------------------
    AsciiFileReader::AsciiFileReader()
	: AsciiReader() {}

    AsciiFileReader::AsciiFileReader(const std::string& filename)
    {
	open(filename);
    }

    void AsciiFileReader::open(const std::string& filename)
    {
	ifs.open(filename.c_str(), std::ifstream::in);
	if(!ifs) throw IOException("AsciiFileReader", filename.c_str());
    }

    void AsciiFileReader::close()
    {
	if(is_open())
	    ifs.close();
    }

    bool AsciiFileReader::is_open()
    {
	return ifs.is_open();
    }

    AsciiFileReader::~AsciiFileReader()
    {
	close();
    }


    
//-----------------------------------------------------------
// Writer
//-----------------------------------------------------------
    Writer::Writer () {}
    

// String writer
    void Writer::write(std::string result)
    {
	getOstream() << result;
    }
    void Writer::write(const char* result)
    {
	getOstream() << result;
    }

// Writers
    void Writer::write(char result)
    {
	writePrimitive<char> (result);
    }
    void Writer::write(int result)
    {
	writePrimitive<int> (result);
    }
    void Writer::write(unsigned int result)
    {
	writePrimitive<unsigned int> (result);
    }
    void Writer::write(short int result)
    {
	writePrimitive<short int> (result);
    }
    void Writer::write(unsigned short int result)
    {
	writePrimitive<unsigned short int> (result);
    }
    void Writer::write(long int result)
    {
	writePrimitive<long int> (result);
    }
    void Writer::write(unsigned long int result)
    {
	writePrimitive<unsigned long int> (result);
    }
    void Writer::write(long long int result)
    {
	writePrimitive<long long int> (result);
    }
    void Writer::write(float result)
    {
	writePrimitive<float> (result);
    }
    void Writer::write(double result)
    {
	writePrimitive<double> (result);
    }
    void Writer::write(bool result)
    {
	writePrimitive<bool> (result);
    }
    bool Writer::writeLine(std::string output)
    {
	std::ostream& os = getOstream();
	os << output << '\n';
	return !os.fail();
    }

    template<typename T>
    void Writer::writePrimitive(T output)
    {
	getOstream() << output;
    }

    Writer& operator<<(Writer& txt, std::string input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, const char* input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, char input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, int input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, unsigned int input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, short int input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, unsigned short int input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, long int input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, unsigned long int input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, long long int input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, float input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, double input) {
	txt.write(input);
	return txt;
    }
    Writer& operator<<(Writer& txt, bool input) {
	txt.write(input);
	return txt;
    }

        
//-----------------------------------------------------------
// AsciiWriter
//-----------------------------------------------------------
    AsciiWriter::AsciiWriter ()
	: sbuf(0), os(sbuf)
    {}

    AsciiWriter::AsciiWriter (std::ostream& out)
	: sbuf(out.rdbuf()), os(sbuf)
    {}

    
//-----------------------------------------------------------
// AsciiFileWriter
//-----------------------------------------------------------
    AsciiFileWriter::AsciiFileWriter()
	: AsciiWriter() {}

    AsciiFileWriter::AsciiFileWriter(const std::string& filename)
    {
	open(filename);
    }

    void AsciiFileWriter::open(const std::string& filename)
    {
	ofs.open(filename.c_str(), std::ofstream::out);
	if(!ofs) throw IOException("AsciiFileWriter", filename.c_str());
    }

    void AsciiFileWriter::close()
    {
	if(is_open())
	    ofs.close();
    }

    bool AsciiFileWriter::is_open()
    {
	return ofs.is_open();
    }

    AsciiFileWriter::~AsciiFileWriter()
    {
	close();
    }


}	//namespace LQCDA


