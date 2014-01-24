/*
 * DataFileParser.hpp
 *
 *  Created on: Jun 14, 2013
 *      Author: Thibaut Metivet
 */

#ifndef DATA_FILE_PARSER_HPP_
#define DATA_FILE_PARSER_HPP_

#include "io.hpp"
#include "DataReader.hpp"
#include "exceptions.hpp"
#include <functional>
#include <algorithm>

namespace LQCDA {

    template<class DataType,
	     class HeaderType,
	     class DataReaderPolicy = DataReader<DataType> >
    class DataFileParser
    {
    public:
	static DataType Parse(const std::string& file, const HeaderType& header) {
	    AsciiFileReader r(file);
	    HeaderType h;
	    while(GetNextHeader(r, h)) {
		if(header == h) {
		    return DataReaderPolicy::Read(&r);
		}
	    }
	    throw IOException("DataFileParser", file.c_str());		// TODO
	}

    private:
	static bool GetNextHeader(Reader& r, HeaderType& header) {
	    std::string buf;
	    while(r.readLine(buf)) {
// trims white space from the beginning of the line
		buf.erase(buf.begin(), find_if(buf.begin(), buf.end(), std::not1(std::ptr_fun<int, int>(isspace))));
		if(buf[0] == '#') {
		    header = HeaderType(buf);
		    return true;
		}
	    }
	    return false;
	}
    };

    template<class DataType,
	     class HeaderType,
	     class DataReaderPolicy = DataReader<DataType> >
    DataType ParseFile(const std::string& file, const HeaderType& header) {
	return DataFileParser<DataType, HeaderType, DataReaderPolicy>::Parse(file, header);
    }

} // namespace LQCDA

#endif	// DATA_FILE_PARSER_HPP_
