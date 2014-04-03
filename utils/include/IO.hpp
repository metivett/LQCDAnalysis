/*
 * IO.hpp
 *
 *  Created on: Apr 02, 2014
 *      Author: Thibaut Metivet
 */

#ifndef IO_HPP
#define IO_HPP

 #include <ostream>

 namespace utils {
 	namespace {
 		template<class charT, class traits = std::char_traits<charT>>
 		class mute_out_buf
 		: public std::basic_streambuf<charT, traits>
 		{
 		public:
 			virtual std::streamsize xsputn(const char* p, std::streamsize n) {
 				return n;
 			}
 			virtual int overflow(int c) {
 				return 1;
 			}
 		};

 		template<class charT, class traits = std::char_traits<charT>>
 		class mute_ostream
 		: public std::basic_ostream<charT, traits>
 		{
 		public:
 			mute_ostream()
 			: std::basic_ostream<charT, traits>(&_mute)
 			{}

 		private:
 			static mute_out_buf<charT, traits> _mute;
 		};
 		template<class charT, class traits>
 		mute_out_buf<charT, traits> mute_ostream<charT, traits>::_mute;
 	}

 	template<class charT, class traits = std::char_traits<charT>>
 	class basic_vostream
 	: public std::basic_ostream<charT, traits>
 	{
 	public:
 		basic_vostream(std::basic_ostream<charT, traits>& os, unsigned int verbosity = 1)
 		: std::basic_ostream<charT, traits>(os.rdbuf())
 		, _Verbosity(verbosity)
 		{}
 		~basic_vostream() = default;

 		void setVerbosity(unsigned int v) { _Verbosity = v; }

 		std::basic_ostream<charT, traits>& operator() (unsigned int v) {
 			if(v <= _Verbosity) {
 				return *this;
 			}
 			else {
 				return mute_out;
 			}
		}

 	private:
 		unsigned int _Verbosity;

 		static mute_ostream<charT, traits> mute_out;
 	};

 	template<class charT, class traits>
 	mute_ostream<charT, traits> basic_vostream<charT, traits>::mute_out;

 	typedef basic_vostream<char> vostream;

 }

#endif // IO_HPP