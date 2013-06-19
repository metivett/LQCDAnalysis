/*
 * DataReader.hpp
 *
 *  Created on: Jun 14, 2013
 *      Author: Thibaut Metivet
 */

#ifndef DATA_READER_HPP_
#define DATA_READER_HPP_

#include "io.hpp"
#include "Eigen/Dense"
#include <sstream>
#include <complex>

namespace LQCDA {

    template<class T>
    class DataReader
    {
    public:
	static T Read(Reader* r) {
	    T result;
	    r->read(result);
	    return result;
	}
    };

template<class T>
class DataReader<std::complex<T> >
    {
    public:
	static std::complex<T> Read(Reader* r) {
	    T re = DataReader<T>::Read(r);
	    T im = DataReader<T>::Read(r);
	    return std::complex<T>(re, im);
	}
    };

    template<class T>
    class DataReader<std::vector<T> >
    {
    public:
	static std::vector<T> Read(Reader* r) {
	    unsigned int n;
	    std::string tmp;
	    r->readLine(tmp);
            std::istringstream dims(tmp);
            dims >> n;
	    std::vector<T> result(n);
	    for(int i = 0; i < n; ++i) {
		result[i] = DataReader<T>::Read(r);
	    }

	    return result;
	}
    };

    template<class T>
    class DataReader<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> >
    {
    public:
	static Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Read(Reader* r) {
	    unsigned int n, m;
	    std::string tmp;
	    r->readLine(tmp);
            std::istringstream dims(tmp);
            dims >> n;
            if(!(dims >> m)) {
                m = 1;
            }
	    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> result(n,m);
	    for(int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
		    result(i,j) = DataReader<T>::Read(r);
		}
	    }

	    return result;
	}
    };

    template<class T>
    class DataReader<Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> >
    {
    public:
	static Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> Read(Reader* r) {
	    unsigned int n, m;
	    std::string tmp;
	    r->readLine(tmp);
            std::istringstream dims(tmp);
            dims >> n;
            if(!(dims >> m)) {
                m = 1;
            }
	    else {
		m /= 2;
	    }
	    Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> result(n,m);
	    for(int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
		    T re = DataReader<T>::Read(r);
		    T im = DataReader<T>::Read(r);
		    result(i,j) = std::complex<T>(re, im);
		}
	    }

	    return result;
	}
    };

    template<class T, int NRow, int NCol>
    class DataReader<Eigen::Matrix<T, NRow, NCol> >
    {
    public:
	static Eigen::Matrix<T, NRow, NCol> Read(Reader* r) {
	    Eigen::Matrix<T, NRow, NCol> result;
	    for(int i = 0; i < NRow; ++i) {
		for(int j = 0; j < NCol; ++j) {
		    result(i,j) = DataReader<T>::Read(r);
		}
	    }

	    return result;
	}
    };

} // namespace LQCDA

#endif	// DATA_READER_HPP_
