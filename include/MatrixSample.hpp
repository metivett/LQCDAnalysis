/*
 * MatrixExprSample.hpp
 *
 *  Created on: Apr 11, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MatrixExpr_SAMPLE_HPP
#define MatrixExpr_SAMPLE_HPP

 #include "Globals.hpp"
 #include "Sample.hpp"

 namespace LQCDA {

 	template<typename T>
 	class Sample<Matrix<T>>
 	: public Array<Matrix<T>, Dynamic, 1>
 	{
 	private:
 		template<typename S>
 		class BlockSampleImpl
 		{
 		private:
 			// Typedefs
 			typedef Block<typename S::nested_type> block_t;
	 		typedef ConstBlock<typename S::nested_type> const_block_t;

 		private:
 			// Data
 			S& _Sample;
 			index_t _i, _j, _nRow, _nCol;

 		public:
 			// Constructors
 			BlockSampleImpl(
 				S& s, 
 				const index_t i, const index_t j, 
 				const index_t nRow, const index_t nCol);
 			// Destructor
 			~BlockSampleImpl() = default;

 			// Accessors
 			unsigned int size() const;

 			// Operators
 			block_t operator[](unsigned int s);
 			const_block_t operator[](unsigned int s) const;
 		};

 	protected:
 		// Typedefs
 		typedef Array<Matrix<T>, Dynamic, 1> Base;
 		typedef Matrix<T> nested_type;

 	public:
 		typedef BlockSampleImpl<Sample<Matrix<T>>> BlockSample;
 		typedef BlockSampleImpl<const Sample<Matrix<T>>> ConstBlockSample;

 	public:
 		// Constructors
 		Sample();
 		explicit Sample(unsigned int size);
 		EIGEN_EXPR_CTOR(Sample, Sample<Matrix<T>>, Base, ArrayExpr)

 		// Destructor
 		virtual ~Sample() = default;

 		// Accessors
 		unsigned int size() const;
 		void resize(unsigned int size);
 		void resizeMatrix(unsigned int nRow, unsigned int nCol);
 		BlockSample block(index_t i, index_t j, unsigned int nRow, unsigned int nCol);
 		ConstBlockSample block(index_t i, index_t j, unsigned int nRow, unsigned int nCol) const;

 		// Operators
 		nested_type& operator[](unsigned int s);
 		const nested_type& operator[](unsigned int s) const;

 		// Statistics
 		nested_type mean(unsigned int begin = 0, int n = -1) const;
 		nested_type covariance(const Sample<Matrix<T>>& sample, unsigned int begin = 0, int n = -1) const;
 		auto covarianceMatrix(const Sample<Matrix<T>>& sample, unsigned int begin, int n) const
 			-> decltype(REDUX::tensorProd(
 				std::declval<Sample<Matrix<T>>>().segment(begin, n).redux(&REDUX::sum<nested_type>),
 				sample.segment(begin, n).redux(&REDUX::sum<nested_type>)));
 		nested_type variance(unsigned int begin = 0, int n = -1) const;
 		auto varianceMatrix(unsigned int begin = 0, int n = -1) const
 			-> decltype(std::declval<Sample<Matrix<T>>>().covarianceMatrix(std::declval<Sample<Matrix<T>>>(), begin, n));
 	};

 	template<typename T>
 	Sample<Matrix<T>>::Sample()
 	: Base()
 	{}

 	template<typename T>
 	Sample<Matrix<T>>::Sample(unsigned int size)
 	: Base(size)
 	{}

 	template<typename T>
 	unsigned int Sample<Matrix<T>>::size() const
 	{
 		return Base::size();
 	}

 	template<typename T>
 	void Sample<Matrix<T>>::resize(unsigned int size)
 	{
 		Base::resize(size);
 	}

 	template<typename T>
 	void Sample<Matrix<T>>::resizeMatrix(unsigned int nRow, unsigned int nCol)
 	{
 		FOR_SAMPLE(*this, s)
 		{
 			(*this)[s].resize(nRow, nCol);
 		}
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::BlockSample Sample<Matrix<T>>::block(index_t i, index_t j, unsigned int nRow, unsigned int nCol)
 	{
 		return Sample<Matrix<T>>::BlockSample(*this, i, j, nRow, nCol);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::ConstBlockSample Sample<Matrix<T>>::block(index_t i, index_t j, unsigned int nRow, unsigned int nCol) const
 	{
 		return Sample<Matrix<T>>::ConstBlockSample(*this, i, j, nRow, nCol);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::nested_type& Sample<Matrix<T>>::operator[](unsigned int s)
 	{
 		return Base::operator[](s);
 	}

 	template<typename T>
 	const typename Sample<Matrix<T>>::nested_type& Sample<Matrix<T>>::operator[](unsigned int s) const 
 	{
 		return Base::operator[](s);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::nested_type Sample<Matrix<T>>::mean(unsigned int begin, int n) const
 	{
 		nested_type result;
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			result = this->segment(begin, len).redux(&REDUX::sum<nested_type>);
 		}
 		return result / static_cast<double>(len);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::nested_type Sample<Matrix<T>>::covariance(const Sample<Matrix<T>>& sample, unsigned int begin, int n) const
 	{
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			auto sampleV = sample.segment(begin, len);
 			auto thisV = this->segment(begin, len);

 			// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
 			return ( thisV.binaryExpr(sampleV, &REDUX::prod<nested_type>).redux(&REDUX::sum<nested_type>) 
 				- REDUX::prod(thisV.redux(&REDUX::sum<nested_type>), sampleV.redux(&REDUX::sum<nested_type>)) / static_cast<double>(len) )
 				/ static_cast<double>(len - 1);
 		}
 	}

 	template<typename T>
 	auto Sample<Matrix<T>>::covarianceMatrix(const Sample<Matrix<T>>& sample, unsigned int begin, int n) const
 	-> decltype(REDUX::tensorProd(
 		std::declval<Sample<Matrix<T>>>().segment(begin, n).redux(&REDUX::sum<nested_type>), 
 		sample.segment(begin, n).redux(&REDUX::sum<nested_type>)))
 	{
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			auto sampleV = sample.segment(begin, len);
 			auto thisV = this->segment(begin, len);

 			// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
 			return ( thisV.binaryExpr(sampleV, &REDUX::tensorProd<nested_type>).redux(&REDUX::sum<nested_type>) 
 				- REDUX::tensorProd(thisV.redux(&REDUX::sum<nested_type>), sampleV.redux(&REDUX::sum<nested_type>)) / static_cast<double>(len) )
 				/ static_cast<double>(len - 1);
 		}
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::nested_type Sample<Matrix<T>>::variance(unsigned int begin, int n) const
 	{
 		return covariance(*this, begin, n);
 	}

 	template<typename T>
 	auto Sample<Matrix<T>>::varianceMatrix(unsigned int begin, int n) const
 	-> decltype(std::declval<Sample<Matrix<T>>>().covarianceMatrix(std::declval<Sample<Matrix<T>>>(), begin, n))
	{
		return covarianceMatrixExpr(*this, begin, n);
	}


	template<typename T>
	template<typename S>
 	Sample<Matrix<T>>::BlockSampleImpl<S>::BlockSampleImpl(
 		S& s, 
 		const index_t i, const index_t j, 
 		const index_t nRow, const index_t nCol)
 	: _Sample(s)
 	, _i(i)
 	, _j(j)
 	, _nRow(nRow)
 	, _nCol(nCol)
 	{}

 	template<typename T>
	template<typename S>
 	unsigned int Sample<Matrix<T>>::BlockSampleImpl<S>::size() const 
 	{
 		return _Sample.size();
 	}

 	template<typename T>
	template<typename S>
 	typename Sample<Matrix<T>>::template BlockSampleImpl<S>::block_t 
 	Sample<Matrix<T>>::BlockSampleImpl<S>::operator[](unsigned int s) 
 	{
 		return _Sample[s].block(_i, _j, _nRow, _nCol);
 	}

 	template<typename T>
	template<typename S>
 	typename Sample<Matrix<T>>::template BlockSampleImpl<S>::const_block_t 
 	Sample<Matrix<T>>::BlockSampleImpl<S>::operator[](unsigned int s) const
 	{
 		return _Sample[s].block(_i, _j, _nRow, _nCol);
 	}

 }

#endif // MatrixExpr_SAMPLE_HPP