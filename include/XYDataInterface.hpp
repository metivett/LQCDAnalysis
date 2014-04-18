/*
 * XYDataInterface.hpp
 *
 *  Created on: May 5, 2013
 *      Author: Thibaut Metivet
 */

#ifndef XY_DATA_INTERFACE_HPP
#define XY_DATA_INTERFACE_HPP

 #include "Globals.hpp"

 #include <initializer_list>

 namespace LQCDA {

/******************************************************************************
 *                           class XYDataInterface                            *
 ******************************************************************************/

 	template<typename T>
 	class XYDataInterface
 	{
 	protected:
 		// Typedefs
 		typedef Block<Matrix<T>> block_t;
 		typedef ConstBlock<Matrix<T>> const_block_t;

 		struct range
 		{
 			unsigned int r[2];
 			unsigned int operator[](unsigned int i) { return r[i]; }
 		};

 	public:
 		// Accessors
 		virtual unsigned int nPoints() const =0;
 		virtual unsigned int xDim() const =0;
 		virtual unsigned int yDim() const =0;

 		virtual T& x(index_t i, index_t k) =0;
 		virtual const T& x(index_t i, index_t k) const =0;
 		virtual block_t x(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) =0;
 		virtual const_block_t x(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) const =0;
 		virtual block_t x(index_t i, std::initializer_list<index_t> r2) =0;
 		virtual const_block_t x(index_t i, std::initializer_list<index_t> r2) const =0;
 		virtual block_t x(std::initializer_list<index_t> r1, index_t k) =0;
 		virtual const_block_t x(std::initializer_list<index_t> r1, index_t k) const =0;

 		virtual T& y(index_t i, index_t k) =0;
 		virtual const T& y(index_t i, index_t k) const =0;
 		virtual block_t y(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) =0;
 		virtual const_block_t y(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) const =0;
 		virtual block_t y(index_t i, std::initializer_list<index_t> r2) =0;
 		virtual const_block_t y(index_t i, std::initializer_list<index_t> r2) const =0;
 		virtual block_t y(std::initializer_list<index_t> r1, index_t k) =0;
 		virtual const_block_t y(std::initializer_list<index_t> r1, index_t k) const =0;

 		virtual block_t xxCov(index_t k1, index_t k2) =0;
 		virtual const_block_t xxCov(index_t k1, index_t k2) const =0;
 		virtual block_t yyCov(index_t k1, index_t k2) =0;
 		virtual const_block_t yyCov(index_t k1, index_t k2) const =0;
 		virtual block_t xyCov(index_t k1, index_t k2) =0;
 		virtual const_block_t xyCov(index_t k1, index_t k2) const =0;

 	};

 }

#endif // XY_DATA_INTERFACE_HPP