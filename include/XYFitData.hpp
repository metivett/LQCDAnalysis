/*
 * XYFitData.hpp
 *
 *  Created on: Mar 28, 2014
 *      Author: Thibaut Metivet
 */

 #ifndef XY_FIT_DATA_HPP
 #define XY_FIT_DATA_HPP

 #include "XYData.hpp"
 #include "FitInterface.hpp"
 
 namespace LQCDA {

 	template<typename T>
 	class XYFitDataBase
 	: public virtual XYDataInterface<T>
 	, public FitInterface
 	{
 	public:
 		// Constructors
 		//using FitInterface::FitInterface;
 		// Constructors
 		XYFitDataBase() = default;
 		XYFitDataBase(unsigned int npts, unsigned int xdim, unsigned int ydim)
 		: FitInterface(npts, xdim, ydim)
 		{}
		// Destructor
 		virtual ~XYFitDataBase() =0;
 		using FitInterface::resize;
 	};

 	template<typename T>
 	XYFitDataBase<T>::~XYFitDataBase() {}


 	template<typename T>
 	class XYFitData
 	: public XYFitDataBase<T>
 	, public XYData<T>
 	{
 	public:
 		// Constructors
 		XYFitData() = default;
 		XYFitData(unsigned int npts, unsigned int xdim, unsigned int ydim);
 		// Destructor
 		virtual ~XYFitData() = default;

 		// Accessors
 		void resize(unsigned int npts, unsigned int xdim, unsigned int ydim);
 	};

 	template<typename T>
 	XYFitData<T>::XYFitData(unsigned int npts, unsigned int xdim, unsigned int ydim)
 	: XYFitDataBase<T>(npts, xdim, ydim)
 	, XYData<T>(npts, xdim, ydim)
 	{}

 	template<typename T>
 	void XYFitData<T>::resize(unsigned int npts, unsigned int xdim, unsigned int ydim)
 	{
 		XYFitDataBase<T>::resize(npts, xdim, ydim);
 		XYData<T>::resize(npts, xdim, ydim);
 	}

 }
 
 #endif // XY_FIT_DATA_HPP