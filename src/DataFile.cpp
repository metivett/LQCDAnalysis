/*
 * DataFile.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Thibaut Metivet
 */

#ifndef DATA_FILE_CPP
#define DATA_FILE_CPP

 #include "DataFile.hpp"
 #include "Exceptions.hpp"

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
 	ERROR(Implementation, "Not implemented yet");
 }

 void AsciiDataFile::saveAs(const std::string& name)
 {
 	ERROR(Implementation, "Not implemented yet");
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
 }

 void AsciiDataFile::closeAsciiFile()
 {
 	if(isOpen())
 	{
 		_FStream.close();
 	}
 	_Name = "";
 	_Mode = FileMode::NONE;
 }

 void AsciiDataFile::parse()
 {
 	_FStream.seekg(0);
 	std::string buf;
 	while(std::getline(_FStream, buf)) {
 		if(buf.compare(0, 6, "#begin") == 0) {
 			std::string name(find_if(buf.begin()+6, buf.end(), std::not1(std::ptr_fun<int, int>(isspace))), buf.end());
 			if(name != "") {
 				int begin_pos = _FStream.tellg();
 				int end_pos = _FStream.tellg();
 				while(std::getline(_FStream, buf)) {
 					if(buf.compare(0, 4, "#end") == 0) {
 						unsigned int len = end_pos - begin_pos + 1;
 						_Data[name] = {begin_pos, len};
 						continue;
 					}
 					end_pos = _FStream.tellg();
 				}
 			}
 		}
 	}
 }

 inline std::fstream& AsciiDataFile::getFstream()
 {
 	return _FStream;
 }

#endif // DATA_FILE_CPP