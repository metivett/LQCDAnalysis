/*
 * XYDataSample.hpp
 *
 *  Created on: Apr 10, 2014
 *      Author: Thibaut Metivet
 */

#ifndef XY_DATA_SAMPLE_HPP
#define XY_DATA_SAMPLE_HPP

 #include "Sample.hpp"
 #include "XYData.hpp"

 namespace LQCDA {

 	template<typename T>
 	class XYDataSample
 	{
 	protected:
 		// Typedefs
 		typedef Block<Matrix<T>> block_t;
 		typedef ConstBlock<Matrix<T>> const_block_t;
 		typedef Sample<Matrix<T>> MatrixSample;
 		typedef typename MatrixSample::BlockSample BlockSample;
 		typedef typename MatrixSample::ConstBlockSample ConstBlockSample;

 		// Data
 		unsigned int _nPts, _xDim, _yDim;
 		MatrixSample _x, _y;
 		Matrix<Matrix<T>> _Cxx, _Cyy, _Cxy;
 		bool is_Cxx_updated, is_Cyy_updated, is_Cxy_updated;

 	public:
 		// Constructors
 		XYDataSample();
 		explicit XYDataSample(
 			unsigned int npts, unsigned int xdim, 
 			unsigned int ydim, unsigned int nsamples);

 		// Destructor
 		virtual ~XYDataSample() = default;

 		// Accessors
 		unsigned int size() const;
 		void resize(
 			unsigned int npts, unsigned int xdim, 
 			unsigned int ydim, unsigned int nsamples);

 		unsigned int nPoints() const { return _nPts; }
 		unsigned int xDim() const { return _xDim; }
 		unsigned int yDim() const { return _yDim; }

 		Sample<T&> x(index_t i, index_t k) const;
 		BlockSample x(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2);
 		ConstBlockSample x(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) const;
 		BlockSample x(index_t i, std::initializer_list<index_t> r2);
 		ConstBlockSample x(index_t i, std::initializer_list<index_t> r2) const;
 		BlockSample x(std::initializer_list<index_t> r1, index_t k);
 		ConstBlockSample x(std::initializer_list<index_t> r1, index_t k) const;

 		Sample<T> y(index_t i, index_t k) const;
 		BlockSample y(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2);
 		ConstBlockSample y(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) const;
 		BlockSample y(index_t i, std::initializer_list<index_t> r2);
 		ConstBlockSample y(index_t i, std::initializer_list<index_t> r2) const;
 		BlockSample y(std::initializer_list<index_t> r1, index_t k);
 		ConstBlockSample y(std::initializer_list<index_t> r1, index_t k) const;

 		const_block_t xxCov(index_t k1, index_t k2) const;
 		const_block_t yyCov(index_t k1, index_t k2) const;
 		const_block_t xyCov(index_t k1, index_t k2) const;

 		// Operators
 		XYData<T>& operator[](unsigned int s);
 		const XYData<T>& operator[](unsigned int s) const;

 		// Statistics
 		// XYData<T> mean(unsigned int begin = 0, int n = -1) const;

 	private:
 		void update_Cxx();
 		void update_Cyy();
 		void update_Cxy();
 	};

 	template<typename T>
 	XYDataSample<T>::XYDataSample()
 	: is_Cxx_updated{false}
 	, is_Cyy_updated{false}
 	, is_Cxy_updated{false}
 	{}

 	template<typename T>
 	XYDataSample<T>::XYDataSample(
 		unsigned int npts, unsigned int xdim, 
 		unsigned int ydim, unsigned int nsamples)
 	: is_Cxx_updated{false}
 	, is_Cyy_updated{false}
 	, is_Cxy_updated{false}
 	{
 		this->resize(npts, xdim ,ydim, nsamples);
 	}

 	template<typename T>
 	void XYDataSample<T>::resize(
 		unsigned int npts, unsigned int xdim, 
 		unsigned int ydim, unsigned int nsamples)
 	{
 		_x.resize(nsamples);
 		_x.resizeMatrix(npts, xdim);
 		_y.resize(nsamples);
 		_y.resizeMatrix(npts, ydim);
 		is_Cxx_updated = false;
		is_Cyy_updated = false;
		is_Cxy_updated = false;
 	}

 	template<typename T>
 	Sample<T&> XYDataSample<T>::x(index_t i, index_t k) const
 	{
 		return Sample<T&>(_x.data());//TODO
 	}

 	template<typename T>
 	typename XYDataSample<T>::BlockSample XYDataSample<T>::x(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2)
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::x(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) const
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::BlockSample XYDataSample<T>::x(index_t i, std::initializer_list<index_t> r2)
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::x(index_t i, std::initializer_list<index_t> r2) const
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::BlockSample XYDataSample<T>::x(std::initializer_list<index_t> r1, index_t k)
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::x(std::initializer_list<index_t> r1, index_t k) const
 	{

 	}

 	template<typename T>
	Sample<T> XYDataSample<T>::y(index_t i, index_t k) const
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::BlockSample XYDataSample<T>::y(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2)
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::y(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) const
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::BlockSample XYDataSample<T>::y(index_t i, std::initializer_list<index_t> r2)
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::y(index_t i, std::initializer_list<index_t> r2) const
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::BlockSample XYDataSample<T>::y(std::initializer_list<index_t> r1, index_t k)
 	{

 	}

 	template<typename T>
 	typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::y(std::initializer_list<index_t> r1, index_t k) const
 	{

 	}


 	// template<typename T>
 	// XYData<T>& XYDataSample<T>::operator[](unsigned int s)
 	// {
 	// 	return Base::operator[](s);
 	// }

 	// template<typename T>
 	// const XYData<T>& XYDataSample<T>::operator[](unsigned int s) const 
 	// {
 	// 	return Base::operator[](s);
 	// }

 	template<typename T>
 	typename XYDataSample<T>::const_block_t XYDataSample<T>::xxCov(index_t k1, index_t k2) const
 	{
 		if(!is_Cxx_updated)
 			update_Cxx();
 		return _Cxx(k1, k2);
 	}

 	template<typename T>
 	typename XYDataSample<T>::const_block_t XYDataSample<T>::yyCov(index_t k1, index_t k2) const
 	{
 		if(!is_Cyy_updated)
 			update_Cxx();
 		return _Cyy(k1, k2);
 	}

 	template<typename T>
 	typename XYDataSample<T>::const_block_t XYDataSample<T>::xyCov(index_t k1, index_t k2) const
 	{
 		if(!is_Cxy_updated)
 			update_Cxx();
 		return _Cxy(k1, k2);
 	}

	template<typename T>
	void XYDataSample<T>::update_Cxx()
	{
		T n = static_cast<T>(size());
		FOR_MAT(_Cxx, k1, k2)
		{
			_Cxx(k1, k2) = ( _x.col(k1).transpose()*_x.col(k2) - _x.col(k1).sum()*_x.col(k2).sum() / n ) / (n - 1);
		}
	}

	template<typename T>
	void XYDataSample<T>::update_Cyy()
	{
		T n = static_cast<T>(size());
		FOR_MAT(_Cyy, k1, k2)
		{
			_Cyy(k1, k2) = ( _y.col(k1).transpose()*_y.col(k2) - _y.col(k1).sum()*_y.col(k2).sum() / n ) / (n - 1);
		}
	}

	template<typename T>
	void XYDataSample<T>::update_Cxy()
	{
		T n = static_cast<T>(size());
		FOR_MAT(_Cxy, k1, k2)
		{
			_Cxy(k1, k2) = ( _x.col(k1).transpose()*_y.col(k2) - _x.col(k1).sum()*_y.col(k2).sum() / n ) / (n - 1);
		}
	}

// template<typename T>
 	// XYData<T> XYDataSample<T>::mean(unsigned int begin, int n) const
 	// {
 	// 	XYData<T> result();
 	// 	const unsigned int len = (n >= 0)? n: size();
 	// 	if(len)
 	// 	{
 	// 		result = this->segment(begin, len).redux(&REDUX::sum<T>);
 	// 	}
 	// 	return result / static_cast<double>(len);
 	// }

 // 	template<typename T>
 // 	T XYDataSample::covariance(const XYDataSample& sample, unsigned int begin, int n) const
 // 	{
 // 		const unsigned int len = (n >= 0)? n: size();
 // 		if(len)
 // 		{
 // 			auto sampleV = sample.segment(begin, len);
 // 			auto thisV = this->segment(begin, len);

 // 			// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
 // 			return ( thisV.binaryExpr(sampleV, &REDUX::prod<T>).redux(&REDUX::sum<T>) 
 // 				- REDUX::prod(thisV.redux(&REDUX::sum<T>), sampleV.redux(&REDUX::sum<T>)) / static_cast<double>(len) )
 // 				/ static_cast<double>(len - 1);
 // 		}
 // 	}

 // 	template<typename T>
 // 	auto XYDataSample::covarianceMatrix(const XYDataSample& sample, unsigned int begin, int n) const
 // 	-> decltype(REDUX::tensorProd(std::declval<XYDataSample>().segment(begin, n).redux(&REDUX::sum<T>), sample.segment(begin, n).redux(&REDUX::sum<T>)))
 // 	{
 // 		const unsigned int len = (n >= 0)? n: size();
 // 		if(len)
 // 		{
 // 			auto sampleV = sample.segment(begin, len);
 // 			auto thisV = this->segment(begin, len);

 // 			// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
 // 			return ( thisV.binaryExpr(sampleV, &REDUX::tensorProd<T>).redux(&REDUX::sum<T>) 
 // 				- REDUX::tensorProd(thisV.redux(&REDUX::sum<T>), sampleV.redux(&REDUX::sum<T>)) / static_cast<double>(len) )
 // 				/ static_cast<double>(len - 1);
 // 		}
 // 	}

 // 	template<typename T>
 // 	T XYDataSample::variance(unsigned int begin, int n) const
 // 	{
 // 		return covariance(*this, begin, n);
 // 	}

 // 	template<typename T>
 // 	auto XYDataSample::varianceMatrix(unsigned int begin, int n) const
 // 	-> decltype(std::declval<XYDataSample>().covarianceMatrix(std::declval<XYDataSample>(), begin, n))
	// {
	// 	return covarianceMatrix(*this, begin, n);
	// }

 }

#endif // XY_DATA_SAMPLE_HPP