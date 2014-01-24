/*
 * DataSetIterator.hpp
 *
 *  Created on: Feb 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef DATA_SET_ITERATOR_HPP
#define DATA_SET_ITERATOR_HPP

 #include <iterator>
 #include "Vector.hpp"

 namespace LQCDA {

 	template<typename T, int XDIM, int YDIM>
 	class DataSet;

 	template<typename T, int XDIM, int YDIM>
 	class DataSetXIterator: public std::iterator<std::random_access_iterator_tag, Vector<T, XDIM>>
 	{
 	private:
 		DataSet<T, XDIM, YDIM> & _DataSet;
 		unsigned int _Current;

 	protected:
 		typedef std::iterator<std::random_access_iterator_tag, Vector<T, XDIM>> iterator_type;

 	public:
 		typedef typename iterator_type::difference_type difference_type;
 		typedef typename iterator_type::reference reference;
 		typedef typename iterator_type::pointer pointer;

 	public:
 		explicit DataSetXIterator(DataSet<T, XDIM, YDIM>& d, unsigned int i): _DataSet(d), _Current(i) {}

 		reference operator*() { return _DataSet._Pts[_Current].X; }
 		pointer operator->() { return &(_DataSet._Pts[_Current].X); }
 		DataSetXIterator& operator++() { ++_Current; return *this; }
 		DataSetXIterator& operator++(int) { return DataSetXIterator(_DataSet, _Current++); }
 		DataSetXIterator& operator--() { --_Current; return *this; }
 		DataSetXIterator& operator--(int) { return DataSetXIterator(_DataSet, _Current--); }

 	};
 	template<typename T, int XDIM, int YDIM>
 	class DataSetYIterator;

 }

#endif // DATA_SET_ITERATOR_HPP