/*
 * ParserSate.hpp
 *
 *  Created on: Apr 15, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PARSER_STATE_HPP
#define PARSER_STATE_HPP

 #include <string>

 namespace LQCDA {

 	template<typename DataT>
 	class ParserState
 	{
 	public:
 		// Stream
		std::istream * stream;
		std::string* streamName;
		// Scanner
		void * scanner;
		// Data
		DataT * data;

 	public:
 		// Constructor
 		ParserState(std::istream* is, std::string* streamName, DataT* data);
 		// Destructor
 		virtual ~ParserState() = default;

 	private:
		// Scanner alloc/free
		virtual void init_scanner() =0;
		virtual void delete_scanner() =0;
 	};

 	template<typename DataT>
 	ParserState<DataT>::ParserState(std::istream* is, std::string* sName, DataT* datap)
 	: stream(is)
 	, streamName(sName)
 	, scanner(nullptr)
 	, data(datap)
 	{}
 	
 }

#endif // PARSER_STATE_HPP