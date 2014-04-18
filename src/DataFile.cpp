/*
 * DataFile.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Thibaut Metivet
 */

#ifndef DATA_FILE_CPP
#define DATA_FILE_CPP

 #include "DataFile.hpp"
 #include "AsciiParser.hpp"

 using namespace LQCDA;

 DataFile::DataFile()
 : _Name()
 , _Mode(FileMode::NONE)
 {}
 DataFile::DataFile(const std::string& name, FileMode mode)
 : _Name(name)
 , _Mode(mode)
 {}
 DataFile::DataFile(const std::string& name, char mode)
 : _Name(name)
 , _Mode(modeFromChar(mode))
 {}

 DataFile::~DataFile()
 {}

 DataFile::FileMode DataFile::modeFromChar(char c)
 {
 	switch (c)
 	{
 		case 'r':
 		return FileMode::READ;
 		case 'w':
 		return FileMode::WRITE;
 		case 'a':
 		return FileMode::APPEND;
 		default:
 		ERROR(IO, "Unsupported data file mode");
 	}
 }

 const DataFile::data_block& DataFile::getData(const std::string& s)
 {
 	std::string dataName = load(s);
 	try
 	{
 		return *_Data.at(dataName);
 	}
 	catch(std::out_of_range) {
 		ERROR(IO, "No data with name \"" + dataName + "\" in file " + _Name);
 	}
 }

 void DataFile::resetData()
 {
 	for(auto &i: _Data)
 	{
 		i.second.reset();
 	}
 	_Data.clear();
 }



 AsciiDataFile::AsciiDataFile(const std::string& name, FileMode mode)
 {
 	openAsciiFile(name, mode);
 }
 AsciiDataFile::AsciiDataFile(const std::string& name, char mode)
 {
 	openAsciiFile(name, modeFromChar(mode));
 }

 bool AsciiDataFile::isOpen() const {
 	return _FStream.is_open();
 }

 void AsciiDataFile::open(const std::string& name, char mode)
 {
 	if(isOpen()) {
 		ERROR(IO, "DataFile already open with filename '" + _Name + "'");
 	}
 	openAsciiFile(name, modeFromChar(mode));
 }

 void AsciiDataFile::close()
 {
 	closeAsciiFile();
 }

 void AsciiDataFile::save()
 {
 	ERROR(IMPLEMENTATION, "Not implemented yet");
 }

 void AsciiDataFile::saveAs(const std::string& name)
 {
 	ERROR(IMPLEMENTATION, "Not implemented yet");
 }

 std::ios_base::openmode fstreamMode(DataFile::FileMode m)
 {
 	switch (m)
 	{
 		case DataFile::FileMode::READ:
 		return std::ios_base::in;
 		case DataFile::FileMode::WRITE:
 		return std::ios_base::out;
 		case DataFile::FileMode::APPEND:
 		return std::ios_base::app;
 		default:
 		ERROR(IO, "Unsupported data file mode");
 	}
 }

 void AsciiDataFile::openAsciiFile(const std::string& name, FileMode mode)
 {
 	_Name = name;
 	_Mode = mode;
 	_FStream.open(name.c_str(), fstreamMode(mode));
 	if(mode & FileMode::READ)
 	{
 		_ParserState.reset(new AsciiParserState(&_FStream, &_Name, &_Data));
 	}
 	else
 	{
 		_ParserState.reset();
 	}
 }

 void AsciiDataFile::closeAsciiFile()
 {
 	if(isOpen())
 	{
 		_FStream.close();
 	}
 	_Name = "";
 	_Mode = FileMode::NONE;
 	_isParsed = false;
 	resetData();
 }

 // void AsciiDataFile::parse()
 // {
 // 	_FStream.seekg(0);
 // 	std::string buf;
 // 	while(std::getline(_FStream, buf)) {
 // 		if(buf.compare(0, 6, "#begin") == 0) {
 // 			std::string name(find_if(buf.begin()+6, buf.end(), std::not1(std::ptr_fun<int, int>(isspace))), buf.end());
 // 			if(name != "") {
 // 				int begin_pos = _FStream.tellg();
 // 				int end_pos = _FStream.tellg();
 // 				while(std::getline(_FStream, buf)) {
 // 					if(buf.compare(0, 4, "#end") == 0) {
 // 						unsigned int len = end_pos - begin_pos + 1;
 // 						_Data[name] = {begin_pos, len};
 // 						continue;
 // 					}
 // 					end_pos = _FStream.tellg();
 // 				}
 // 			}
 // 		}
 // 	}
 // }

 std::string AsciiDataFile::load(const std::string& s)
 {
 	if((_Mode & FileMode::READ) && isOpen())
 	{
 		if(!_isParsed)
 		{
 			parse();
 		}
 		return s;
 	}
 	else
 	{
 		if(isOpen())
 		{
 			ERROR(IO, "file \"" + _Name + "\" not opened in read mode");
 		}
 		else
 		{
 			ERROR(IO, "no file opened");
 		}
 	}
 }

 inline std::fstream& AsciiDataFile::getStream()
 {
 	return _FStream;
 }

 int AsciiDataFile::parse()
 {
 	_FStream.seekg(0);
 	yy::AsciiParser parser(_ParserState.get());
 	parser.set_debug_level(false);
 	int res = parser.parse();
 	std::cout << "Parsed " << _Data.size() << " data blocks" << std::endl;
 	_isParsed = true;
 	return res;
 }

#endif // DATA_FILE_CPP