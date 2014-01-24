/*
 * DataSet.hpp
 *
 *  Created on: Apr 15, 2013
 *      Author: Thibaut Metivet
 */

#ifndef DATA_SET_HPP_
#define DATA_SET_HPP_

 #include <vector>
 

 namespace LQCDA {

 	// template<typename T, int XDIM, int YDIM>
 	// class DataSet;

  //   template<typename T, int XDIM, int YDIM>
 	// class DataSet//<T, XDIM, YDIM>
 	// {
 	// private:
 	// 	typedef Point<T, XDIM, YDIM> PointT;
 	// 	// friend class DataSetXIterator<T, XDIM, YDIM>;
 	// 	// friend class DataSetYIterator<T, XDIM, YDIM>;

 	// 	std::vector<PointT> _Pts;

 	// public:
 	// 	DataSet() {}
 	// 	DataSet(unsigned int n): _Pts(n) {}

		// // Getters
 	// 	unsigned int size() const { return _Pts.size(); }

 	// 	const Vector<T, XDIM>& x(unsigned int i) const { return _Pts[i].X; }
 	// 	const Vector<T, YDIM>& y(unsigned int i) const { return _Pts[i].Y; }

		// // Setters
 	// 	void setPoint(unsigned int i, std::initializer_list<T> x, std::initializer_list<T> y) {
 	// 		assert(i < size());
 	// 		_Pts[i] = {x, y};
 	// 	}

 	// 	void addPoint(std::initializer_list<T> x, std::initializer_list<T> y) {
 	// 		assert(x.size() == XDIM);
 	// 		assert(y.size() == YDIM);
 	// 		_Pts.emplace_back(x, y);
 	// 	}

 	// 	void reserve(unsigned int n) {
 	// 		_Pts.reserve(n);
 	// 	}

 	// };

} // namespace LQCDA


#endif	// DATA_SET_HPP_
