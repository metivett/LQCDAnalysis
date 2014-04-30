/*
 * XYDataMap.hpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Thibaut Metivet
 */

#ifndef XY_DATA_MAP_HPP
#define XY_DATA_MAP_HPP

 #include "XYDataInterface.hpp"
 #include "Exceptions.hpp"

 namespace LQCDA {

	template<typename T>
 	class XYDataMap
 	: public virtual XYDataInterface<T>
 	{
 	private:
	 	// Typedefs
 		typedef typename XYDataInterface<T>::range range;
 		typedef typename Map<Matrix<T>>::PointerType PointerType;

 	protected:
	 	// Typedefs
 		typedef Ref<Matrix<T>> block_t;
 		typedef ConstRef<Matrix<T>> const_block_t;

	 	// Data
 		unsigned int _nPts, _xDim, _yDim;
 		Map<Matrix<T>> _x, _y;
 		Matrix<Matrix<T>> * _Cxx, * _Cyy, * _Cxy;

 	public:
 		XYDataMap(PointerType x, PointerType y, unsigned int npts, unsigned int xdim, unsigned int ydim);
 		XYDataMap(const XYDataMap<T>& other);

 		void setXXCov(Matrix<Matrix<double>> * Cxx);
 		void setYYCov(Matrix<Matrix<double>> * Cyy);
 		void setXYCov(Matrix<Matrix<double>> * Cxy);

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
 		range check_range(std::initializer_list<index_t> r, unsigned int max) const;

 	private:
 		XYDataMap() {}
 	};

	template<typename T>
 	XYDataMap<T>::XYDataMap(
 		XYDataMap<T>::PointerType x, XYDataMap<T>::PointerType y,
 		unsigned int npts, unsigned int xdim, unsigned int ydim)
 	: _x(x, npts, xdim)
 	, _y(y, npts, ydim)
 	, _Cxx{nullptr}
 	, _Cyy{nullptr}
 	, _Cxy{nullptr}
 	, _nPts{npts}
 	, _xDim{xdim}
 	, _yDim{ydim}
 	{}

	template<typename T>
 	XYDataMap<T>::XYDataMap(const XYDataMap<T>& other)
 	: _x(other._x)
 	, _y(other._y)
 	, _Cxx{other._Cxx}
 	, _Cyy{other._Cyy}
 	, _Cxy{other._Cxy}
 	, _nPts{other._nPts}
 	, _xDim{other._xDim}
 	, _yDim{other._yDim}
 	{}

	template<typename T>
 	void XYDataMap<T>::setXXCov(Matrix<Matrix<double>> * Cxx)
 	{
 		_Cxx = Cxx;
 	}

	template<typename T>
 	void XYDataMap<T>::setYYCov(Matrix<Matrix<double>> * Cyy)
 	{
 		_Cyy = Cyy;
 	}

	template<typename T>
 	void XYDataMap<T>::setXYCov(Matrix<Matrix<double>> * Cxy)
 	{
 		_Cxy = Cxy;
 	}


	template<typename T>
 	T& XYDataMap<T>::x(index_t i, index_t k)
 	{
 		return _x(i, k);
 	}
	template<typename T>
 	const T& XYDataMap<T>::x(index_t i, index_t k) const
 	{
 		return _x(i, k);
 	}
	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::x(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2)
 	{
 		auto r1 = check_range(l1, _nPts);
 		auto r2 = check_range(l2, _xDim);
 		return _x.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::x(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2) const
 	{
 		auto r1 = check_range(l1, _nPts);
 		auto r2 = check_range(l2, _xDim);
 		return _x.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
 	}
	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::x(index_t i, std::initializer_list<index_t> l2)
 	{
 		auto r2 = check_range(l2, _xDim);
 		return _x.block(i, r2[0], 1, r2[1] - r2[0]);
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::x(index_t i, std::initializer_list<index_t> l2) const
 	{
 		auto r2 = check_range(l2, _xDim);
 		return _x.block(i, r2[0], 1, r2[1] - r2[0]);
 	}
	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::x(std::initializer_list<index_t> l1, index_t k)
 	{
 		auto r1 = check_range(l1, _nPts);
 		return _x.block(r1[0], k, r1[1] - r1[0], 1);
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::x(std::initializer_list<index_t> l1, index_t k) const
 	{
 		auto r1 = check_range(l1, _nPts);
 		return _x.block(r1[0], k, r1[1] - r1[0], 1);
 	}

	template<typename T>
 	T& XYDataMap<T>::y(index_t i, index_t k)
 	{
 		return _y(i, k);
 	}
	template<typename T>
 	const T& XYDataMap<T>::y(index_t i, index_t k) const
 	{
 		return _y(i, k);
 	}
	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::y(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2)
 	{
 		auto r1 = check_range(l1, _nPts);
 		auto r2 = check_range(l2, _yDim);
 		return _y.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::y(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2) const
 	{
 		auto r1 = check_range(l1, _nPts);
 		auto r2 = check_range(l2, _yDim);
 		return _y.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
 	}
	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::y(index_t i, std::initializer_list<index_t> l2)
 	{
 		auto r2 = check_range(l2, _yDim);
 		return _y.block(i, r2[0], 1, r2[1] - r2[0]);
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::y(index_t i, std::initializer_list<index_t> l2) const
 	{
 		auto r2 = check_range(l2, _yDim);
 		return _y.block(i, r2[0], 1, r2[1] - r2[0]);
 	}
	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::y(std::initializer_list<index_t> l1, index_t k)
 	{
 		auto r1 = check_range(l1, _nPts);
 		return _y.block(r1[0], k, r1[1] - r1[0], 1);
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::y(std::initializer_list<index_t> l1, index_t k) const
 	{
 		auto r1 = check_range(l1, _nPts);
 		return _y.block(r1[0], k, r1[1] - r1[0], 1);
 	}

	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::xxCov(index_t k1, index_t k2)
 	{
 		if(!_Cxx)
 			ERROR(NULLPTR, "no Cxx matrix provided");
 		return (*_Cxx)(k1, k2).block(0, 0, (*_Cxx)(k1, k2).rows(), (*_Cxx)(k1, k2).cols());
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::xxCov(index_t k1, index_t k2) const
 	{
 		if(!_Cxx)
 			ERROR(NULLPTR, "no Cxx matrix provided");
 		return (*_Cxx)(k1, k2).block(0, 0, (*_Cxx)(k1, k2).rows(), (*_Cxx)(k1, k2).cols());
 	}
	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::yyCov(index_t k1, index_t k2)
 	{
 		if(!_Cyy)
 			ERROR(NULLPTR, "no Cxx matrix provided");
 		return (*_Cyy)(k1, k2).block(0, 0, (*_Cyy)(k1, k2).rows(), (*_Cyy)(k1, k2).cols());
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::yyCov(index_t k1, index_t k2) const
 	{
 		if(!_Cyy)
 			ERROR(NULLPTR, "no Cxx matrix provided");
 		return (*_Cyy)(k1, k2).block(0, 0, (*_Cyy)(k1, k2).rows(), (*_Cyy)(k1, k2).cols());
 	}
	template<typename T>
 	typename XYDataMap<T>::block_t XYDataMap<T>::xyCov(index_t k1, index_t k2)
 	{
 		if(!_Cxy)
 			ERROR(NULLPTR, "no Cxx matrix provided");
 		return (*_Cxy)(k1, k2).block(0, 0, (*_Cxy)(k1, k2).rows(), (*_Cxy)(k1, k2).cols());
 	}
	template<typename T>
 	typename XYDataMap<T>::const_block_t XYDataMap<T>::xyCov(index_t k1, index_t k2) const
 	{
 		if(!_Cxy)
 			ERROR(NULLPTR, "no Cxx matrix provided");
 		return (*_Cxy)(k1, k2).block(0, 0, (*_Cxy)(k1, k2).rows(), (*_Cxy)(k1, k2).cols());
 	}


	template<typename T>
 	typename XYDataMap<T>::range XYDataMap<T>::check_range(std::initializer_list<index_t> r, unsigned int max) const
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


#endif // XY_DATA_MAP_HPP