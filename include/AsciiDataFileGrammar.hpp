/*
 * AsciiDataFileGrammar.hpp
 *
 *  Created on: Feb 27, 2014
 *      Author: Thibaut Metivet
 */

#ifndef ASCII_DATA_FILE_GRAMMAR_HPP
#define ASCII_DATA_FILE_GRAMMAR_HPP

 #include <boost/spirit/include/qi.hpp>
 #include <boost/spirit/include/phoenix_core.hpp>
 #include <boost/spirit/include/phoenix_operator.hpp>
 #include <string>
 #include "DataFile.hpp"

 namespace LQCDA {
 	namespace spirit = boost::spirit;
 	namespace qi = boost::spirit::qi;
 	namespace ascii = boost::spirit::ascii;

 	class AsciiDataFileGrammar
 	: public qi::grammar<spirit::istream_iterator, DataFile::DataTable()>
 	{
 	public:
 		typedef spirit::istream_iterator iterator_type;
 	public:
 		AsciiDataFileGrammar()
 		: AsciiDataFileGrammar::base_type(start)
 		{
 			begin_block = qi::lit("#begin");
 			data = *(qi::double_ >> ascii::space);
 			end_block = (qi::lit("#end") >> ascii::space);
 			data_block = 
 				begin_block
 				>> data
 				>> end_block
 				;
 			start = *data_block;
 		}

 	private:
 		qi::rule<iterator_type, DataFile::DataTable()> start;
 		qi::rule<iterator_type, void()> data_block;
 		qi::rule<iterator_type, std::string()> begin_block;
 		qi::rule<iterator_type, void()> data;
 		qi::rule<iterator_type, void(std::string)> end_block;
 	};

 }

#endif // ASCII_DATA_FILE_GRAMMAR_HPP