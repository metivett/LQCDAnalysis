/*
 * XYData.hpp
 *
 *  Created on: May 5, 2013
 *      Author: Thibaut Metivet
 */

#ifndef XY_DATA_HPP
#define XY_DATA_HPP

 #include "XYDataInterface.hpp"

 namespace LQCDA {

 	template<typename T>
 	class XYData
 	: public virtual XYDataInterface<T>
 	{
 	private:
 		// Typedefs
 		typedef typename XYDataInterface<T>::range range;

 	protected:
 		typedef typename XYDataInterface<T>::block_t block_t;
 		typedef typename XYDataInterface<T>::const_block_t const_block_t;

 		// Data
 		unsigned int _nPts, _xDim, _yDim;
 		Matrix<double> _x, _y;
 		Matrix<Matrix<double>> _Cxx, _Cyy, _Cxy;

 	public:
 		// Constructors
 		XYData() = default;
 		XYData(unsigned int npts, unsigned int xdim, unsigned int ydim);
 		// Destructor
 		virtual ~XYData() = default;

 		// Accessors
 		void resize(unsigned int npts, unsigned int xdim, unsigned int ydim);
 		virtual unsigned int nPoints() const override { return _nPts; }
 		virtual unsigned int xDim() const override { return _xDim; }
 		virtual unsigned int yDim() const override { return _yDim; }

 		virtual T& x(index_t i, index_t k) override;
 		virtual const T& x(index_t i, index_t k) const override;
 		virtual block_t x(std::initializer_list<index_t> r1 = {}, std::initializer_list<index_t> r2 = {}) override;
 		virtual const_block_t x(std::initializer_list<index_t> r1 = {}, std::initializer_list<index_t> r2 = {}) const override;
 		virtual block_t x(index_t i, std::initializer_list<index_t> r2 = {}) override;
 		virtual const_block_t x(index_t i, std::initializer_list<index_t> r2 = {}) const override;
 		virtual block_t x(std::initializer_list<index_t> r1, index_t k) override;
 		virtual const_block_t x(std::initializer_list<index_t> r1, index_t k) const override;

 		virtual T& y(index_t i, index_t k) override;
 		virtual const T& y(index_t i, index_t k) const override;
 		virtual block_t y(std::initializer_list<index_t> r1 = {}, std::initializer_list<index_t> r2 = {}) override;
 		virtual const_block_t y(std::initializer_list<index_t> r1 = {}, std::initializer_list<index_t> r2 = {}) const override;
 		virtual block_t y(index_t i, std::initializer_list<index_t> r2 = {}) override;
 		virtual const_block_t y(index_t i, std::initializer_list<index_t> r2 = {}) const override;
 		virtual block_t y(std::initializer_list<index_t> r1, index_t k) override;
 		virtual const_block_t y(std::initializer_list<index_t> r1, index_t k) const override;

 		virtual block_t xxCov(index_t k1, index_t k2) override;
 		virtual const_block_t xxCov(index_t k1, index_t k2) const override;
 		virtual block_t yyCov(index_t k1, index_t k2) override;
 		virtual const_block_t yyCov(index_t k1, index_t k2) const override;
 		virtual block_t xyCov(index_t k1, index_t k2) override;
 		virtual const_block_t xyCov(index_t k1, index_t k2) const override;

 	protected:
 		void init(unsigned int npts, unsigned int xdim, unsigned int ydim);
 		range check_range(std::initializer_list<index_t> r, unsigned int max) const;

 	};

 	template<typename T>
 	XYData<T>::XYData(unsigned int npts, unsigned int xdim, unsigned int ydim)
 	{
 		init(npts, xdim, ydim);
 	}

 	template<typename T>
 	void XYData<T>::init(unsigned int npts, unsigned int xdim, unsigned int ydim)
 	{
 		_x.resize(npts, xdim);
 		_y.resize(npts, ydim);
 		_Cxx.resize(xdim, xdim);
 		_Cyy.resize(ydim, ydim);
 		_Cxy.resize(xdim, ydim);
 		// FOR_MAT_DIAG(_Cxx, i) {
 		// 	_Cxx(i, i).setIdentity(npts, npts);
 		// }
 		// FOR_MAT_DIAG(_Cyy, i) {
 		// 	_Cyy(i, i).setIdentity(npts, npts);
 		// }
 		FOR_MAT(_Cxx, i, j) {
 			if(i == j)
 				_Cxx(i, j).setIdentity(npts, npts);
 			else
 				_Cxx(i, j).setZero(npts, npts);
 		}
 		FOR_MAT(_Cyy, i, j) {
 			if(i == j)
 				_Cyy(i, j).setIdentity(npts, npts);
 			else
 				_Cyy(i, j).setZero(npts, npts);
 		}
 		FOR_MAT(_Cxy, i, j) {
 			_Cxy(i, j).setZero(npts, npts);
 		}
 		_nPts = npts;
 		_xDim = xdim;
 		_yDim = ydim;
 	}

 	template<typename T>
 	void XYData<T>::resize(unsigned int npts, unsigned int xdim, unsigned int ydim)
 	{
 		_x.resize(npts, xdim);
 		_y.resize(npts, ydim);
 		_Cxx.resize(xdim, xdim);
 		_Cyy.resize(ydim, ydim);
 		_Cxy.resize(xdim, ydim);
 		FOR_MAT(_Cxx, i, j) {
 			_Cxx(i, j).resize(npts, npts);
 		}
 		FOR_MAT(_Cyy, i, j) {
 			_Cyy(i, j).resize(npts, npts);
 		}
 		FOR_MAT(_Cxy, i, j) {
 			_Cxy(i, j).resize(npts, npts);
 		}
 		_nPts = npts;
 		_xDim = xdim;
 		_yDim = ydim;
 	}

 	template<typename T>
 	T& XYData<T>::x(index_t i, index_t k)
 	{
 		return _x(i, k);
 	}
 	template<typename T>
 	const T& XYData<T>::x(index_t i, index_t k) const
 	{
 		return _x(i, k);
 	}
 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::x(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2)
 	{
 		auto r1 = check_range(l1, _nPts);
 		auto r2 = check_range(l2, _xDim);
 		return _x.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::x(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2) const
 	{
 		auto r1 = check_range(l1, _nPts);
 		auto r2 = check_range(l2, _xDim);
 		return _x.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
 	}
 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::x(index_t i, std::initializer_list<index_t> l2)
 	{
 		auto r2 = check_range(l2, _xDim);
 		return _x.block(i, r2[0], 1, r2[1] - r2[0]);
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::x(index_t i, std::initializer_list<index_t> l2) const
 	{
 		auto r2 = check_range(l2, _xDim);
 		return _x.block(i, r2[0], 1, r2[1] - r2[0]);
 	}
 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::x(std::initializer_list<index_t> l1, index_t k)
 	{
 		auto r1 = check_range(l1, _nPts);
 		return _x.block(r1[0], k, r1[1] - r1[0], 1);
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::x(std::initializer_list<index_t> l1, index_t k) const
 	{
 		auto r1 = check_range(l1, _nPts);
 		return _x.block(r1[0], k, r1[1] - r1[0], 1);
 	}

 	template<typename T>
 	T& XYData<T>::y(index_t i, index_t k)
 	{
 		return _y(i, k);
 	}
 	template<typename T>
 	const T& XYData<T>::y(index_t i, index_t k) const
 	{
 		return _y(i, k);
 	}
 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::y(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2)
 	{
 		auto r1 = check_range(l1, _nPts);
 		auto r2 = check_range(l2, _yDim);
 		return _y.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::y(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2) const
 	{
 		auto r1 = check_range(l1, _nPts);
 		auto r2 = check_range(l2, _yDim);
 		return _y.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
 	}
 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::y(index_t i, std::initializer_list<index_t> l2)
 	{
 		auto r2 = check_range(l2, _yDim);
 		return _y.block(i, r2[0], 1, r2[1] - r2[0]);
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::y(index_t i, std::initializer_list<index_t> l2) const
 	{
 		auto r2 = check_range(l2, _yDim);
 		return _y.block(i, r2[0], 1, r2[1] - r2[0]);
 	}
 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::y(std::initializer_list<index_t> l1, index_t k)
 	{
 		auto r1 = check_range(l1, _nPts);
 		return _y.block(r1[0], k, r1[1] - r1[0], 1);
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::y(std::initializer_list<index_t> l1, index_t k) const
 	{
 		auto r1 = check_range(l1, _nPts);
 		return _y.block(r1[0], k, r1[1] - r1[0], 1);
 	}

 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::xxCov(index_t k1, index_t k2)
 	{
 		return _Cxx(k1, k2).block(0, 0, _Cxx(k1, k2).rows(), _Cxx(k1, k2).cols());
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::xxCov(index_t k1, index_t k2) const
 	{
 		return _Cxx(k1, k2).block(0, 0, _Cxx(k1, k2).rows(), _Cxx(k1, k2).cols());
 	}
 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::yyCov(index_t k1, index_t k2)
 	{
 		return _Cyy(k1, k2).block(0, 0, _Cyy(k1, k2).rows(), _Cyy(k1, k2).cols());
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::yyCov(index_t k1, index_t k2) const
 	{
 		return _Cyy(k1, k2).block(0, 0, _Cyy(k1, k2).rows(), _Cyy(k1, k2).cols());
 	}
 	template<typename T>
 	typename XYData<T>::block_t XYData<T>::xyCov(index_t k1, index_t k2)
 	{
 		return _Cxy(k1, k2).block(0, 0, _Cxy(k1, k2).rows(), _Cxy(k1, k2).cols());
 	}
 	template<typename T>
 	typename XYData<T>::const_block_t XYData<T>::xyCov(index_t k1, index_t k2) const
 	{
 		return _Cxy(k1, k2).block(0, 0, _Cxy(k1, k2).rows(), _Cxy(k1, k2).cols());
 	}

	template<typename T>
 	typename XYData<T>::range XYData<T>::check_range(std::initializer_list<index_t> r, unsigned int max) const
 	{
 		unsigned int range_size = r.size();
 		ASSERT(range_size <= 2);
 		range res;
 		switch(range_size) {
 			case 0:
 			res.r[0] = 0;
 			res.r[1] = max;
 			break;
 			case 1:
 			ASSERT(*(r.begin()) < max);
 			res.r[0] = res.r[1] = *(r.begin());
 			break;
 			case 2:
 			std::copy(r.begin(), r.end(), res.r);
 			ASSERT(res.r[1] < max);
 			break;
 		}
 		return res;
 	}

 }

#endif // XY_DATA_HPP