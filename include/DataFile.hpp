/*
 * DataFile.hpp
 *
 *  Created on: Feb 12, 2014
 *      Author: Thibaut Metivet
 */

#ifndef DATA_FILE_HPP
#define DATA_FILE_HPP

 #include <string>
 #include <fstream>
 #include <unordered_map>

 #include "DataReader.hpp"
 #include "Exceptions.hpp"

 namespace LQCDA {

 	namespace internal {
 		class substreambuf: public std::streambuf
 		{
 		private:
 			std::streambuf * _SBuf;
 			std::streampos _Pos;
 			std::streamsize _Len;
 			std::streampos _Read;

 		public:
 			substreambuf(std::streambuf * sbuf, int pos, unsigned int len)
 			: _SBuf(sbuf)
 			, _Pos(pos)
 			, _Len(len)
 			, _Read(0)
 			{
 				_SBuf->pubseekpos(pos);
 				setbuf(NULL, 0);
 			}

 		protected:
 			std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
 				std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) {
 				switch(way) {
 					case std::ios_base::beg:
 					off += _Pos;
 					break;
 					case std::ios_base::cur:
 					off += _Pos + _Read;
 					break;
 					case std::ios_base::end:
 					off += _Pos + _Len;
 					break;
 				}

 				return _SBuf->pubseekpos(off, which) - _Pos;
 			}

 			std::streampos seekpos(std::streampos sp,
 				std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) {
 				sp += _Pos;
 				if(sp > _Pos + _Len)
 					return -1;

 				return _SBuf->pubseekpos(sp, which) - _Pos;
 			}

 			int underflow() {
 				if(_Read >= _Len)
 					return traits_type::eof();
 				return _SBuf->sgetc();
 			}

 			int uflow() {
 				if(_Read >= _Len)
 					return traits_type::eof();
 				_Read += 1;
 				return _SBuf->snextc();
 			}
 		};
 	}
 	
/******************************************************************************
 *                            Base DataFile class                             *
 ******************************************************************************/

 	class DataFile
 	{
 	public:
 		// Enums
 		enum FileMode {
 			NONE = 0,
 			READ = 1 << 0,
 			WRITE = 1 << 1,
 			APPEND = 1 << 2
 		};

 		struct data_position
 		{
 			int pos;
 			unsigned int len;
 		};

 		// Typedefs
 		typedef std::unordered_map<std::string, data_position> DataTable;

 	protected:
 		std::string _Name;
 		FileMode _Mode;

 		DataTable _Data;

 	public:
 		DataFile();
 		DataFile(const std::string& name, FileMode mode);
 		DataFile(const std::string& name, char mode);

 		virtual ~DataFile();

 		std::string getName() const { return _Name; }
 		FileMode getMode() const { return _Mode; }

 		virtual bool isOpen() const =0;

 		virtual void open(const std::string& name, char mode) =0;
 		virtual void close() =0;
 		virtual void save() =0;
 		virtual void saveAs(const std::string&) =0;

 		template<typename T, typename Reader = DefaultDataReader<T>>
 		void read(const std::string& s, T& t, unsigned int offset =0);

 	protected:
 		FileMode modeFromChar(char c);

 		virtual std::fstream& getFstream() =0;
 	};

 	template<typename T, typename Reader>
 	void DataFile::read(const std::string& s, T& t, unsigned int offset)
 	{
 		try {
 			data_position& data_pos = _Data.at(s);
 			int pos = data_pos.pos + offset;
 			int len = data_pos.len - offset;
 			if(len <= 0)
 				ERROR(IO, "No data at offset " + utils::strFrom(offset) 
 					+ " with name " + s + " in file " + _Name);

 			internal::substreambuf ssbuf(getFstream().rdbuf(), pos, len);
 			std::istream sis(&ssbuf);
 			Reader::read(t, sis);
 		}
 		catch(std::out_of_range) {
 			ERROR(IO, "No data with name \"" + s + "\" in file " + _Name);
 		}
 	}

/******************************************************************************
 *                            Ascii DataFile class                            *
 ******************************************************************************/

 	class AsciiDataFile: public DataFile
 	{
 	private:
 		std::fstream _FStream;

 	public:
 		AsciiDataFile(): DataFile() {}
 		AsciiDataFile(const std::string& name, FileMode mode);
 		AsciiDataFile(const std::string& name, char mode);

 		virtual bool isOpen() const override;

 		virtual void open(const std::string& name, char mode) override;
 		virtual void close() override;
 		virtual void save() override;
 		virtual void saveAs(const std::string&) override;

 	private:
 		void openAsciiFile(const std::string& name, FileMode mode);
 		void closeAsciiFile();

 		void parse();

 	protected:
 		virtual std::fstream& getFstream() override;

 	};

 }

#endif // DATA_FILE_HPP