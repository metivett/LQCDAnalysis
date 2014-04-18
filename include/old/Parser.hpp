/*
 * Parser.hpp
 *
 *  Created on: Feb 26, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PARSER_HPP
#define PARSER_HPP

 #include <boost/spirit/include/qi.hpp>
 #include <istream>

 namespace LQCDA {
 	namespace qi = boost::spirit::qi;
 	namespace ascii = boost::spirit::ascii;

 	template<typename Iterator, typename Signature, typename Skipper, typename Locals>
 	class Parser
 	{
 	private:
 		qi::grammar<Iterator, Signature, Skipper, Locals> _G;

 	public:
 		Parser(const qi::grammar<Iterator, Signature, Skipper, Locals>& g)
 		: _G(g)
 		{}

 		template<typename Attribute>
 		bool parse(Iterator & begin, Iterator end, Attribute & attr)
 		{
 			return phrase_parse(begin, end, _G, Skipper(), attr);
 		}

 		template<typename Attribute>
 		bool parse(std::istream& is, Attribute& attr)
 		{
 			return (is >> phrase_match(_G, Skipper(), attr));
 		}
 	};

 }

#endif // PARSER_HPP