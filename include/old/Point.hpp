/*
 * Point.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Thibaut Metivet
 */

#ifndef POINT_HPP
#define POINT_HPP

 #include "Vector.hpp"

 namespace LQCDA {

 	template<typename T, int XDIM, int YDIM>
 	class Point
 	{
 	public:
 		typedef Vector<T, XDIM> x_type;
 		typedef Vector<T, YDIM> y_type;

 		x_type X;
 		y_type Y;

 	public:
 		Point() {}
 		Point(const Point& pt): X(pt.X), Y(pt.Y) {}
 		Point(const Vector<T, XDIM>& x, const Vector<T, YDIM>& y): X(x), Y(y) {}
 		Point(std::initializer_list<T> x, std::initializer_list<T> y): X(x.size()), Y(y.size())
 		{
 			for(int k=0; k<x.size(); k++) X = x.begin()[k];
			for(int k=0; k<y.size(); k++) Y = y.begin()[k];
 		}

 		unsigned int xDim() const { return X.size(); }
 		unsigned int yDim() const { return Y.size(); }

 		Point& operator= (const Point& pt) { X = pt.X; Y = pt.Y; return *this; }
 		Point& operator= (std::initializer_list<T> coords[2])
 		{
 			assert(coords[0].size() == xDim());
			assert(coords[1].size() == yDim());
			for(int k=0; k<coords[0].size(); k++) X(k) = coords[0].begin()[k];
			for(int k=0; k<coords[1].size(); k++) Y(k) = coords[1].begin()[k];
 		}

 	};

 }

#endif // POINT_HPP