/*
 * IOObject.hpp
 *
 *  Created on: Apr 14, 2014
 *      Author: Thibaut Metivet
 */

#ifndef IO_OBJECT_HPP
#define IO_OBJECT_HPP

 namespace LQCDA {

 	class IOObject
 	{
 	public:
 		enum class IOType
 		{
 			MATRIX
 		};

 	public:
 		// Destructor
 		virtual ~IOObject() = default;

 		// Type
 		virtual IOType getIOType() const =0;
 	};

 	template<typename T>
 	class IOTraits;

 	template<typename T>
 	class IO
 	: public T
 	, public IOObject
 	{
 	public:
 		using T::T;

 		virtual IOType getIOType() const
 		{
 			return IOTraits<T>::type;
 		}
 	};

 }

#endif // IO_OBJECT_HPP