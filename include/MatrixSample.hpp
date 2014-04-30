/*
 * MatrixExprSample.hpp
 *
 *  Created on: Apr 11, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MatrixExpr_SAMPLE_HPP
#define MatrixExpr_SAMPLE_HPP

 #include "Globals.hpp"
 #include "TypeTraits.hpp"
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
 			typedef typename 
 				if_<std::is_const<S>::value, 
 				const typename S::SampleType, typename S::SampleType>::result SampleType;
 			typedef typename std::remove_const<S>::type NonConstSType;
 			typedef Block<SampleType> BlockType;
	 		typedef ConstBlock<SampleType> ConstBlockType;

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
 			BlockSampleImpl(BlockSampleImpl<NonConstSType> & b);
 			BlockSampleImpl(BlockSampleImpl<NonConstSType> && b);
 			// Destructor
 			~BlockSampleImpl() = default;

 			// Accessors
 			S& sample();
 			const S& sample() const;
 			unsigned int size() const;
 			unsigned int rows() const;
 			unsigned int cols() const;
 			index_t startRow() const;
 			index_t startCol() const;

 			// Operators
 			BlockType operator[](unsigned int s);
 			ConstBlockType operator[](unsigned int s) const;

 		};

 		template<typename S>
 		class ScalarSampleImpl
 		{
 		private:
 			// Typedefs
 			typedef typename S::Scalar Scalar;

 		private:
 			// Data
 			S& _Sample;
 			index_t _i, _j;

 		public:
 			// Constructors
 			ScalarSampleImpl(
 				S& s, 
 				const index_t i, const index_t j);
 			// Destructor
 			~ScalarSampleImpl() = default;

 			// Accessors
 			unsigned int size() const;

 			// Operators
 			Scalar operator[](unsigned int s);
 			const Scalar operator[](unsigned int s) const;

 		};

 	protected:
 		// Typedefs
 		typedef Array<Matrix<T>, Dynamic, 1> Base;
 		typedef T Scalar;
 		typedef Matrix<T> SampleType;

 	public:
 		typedef BlockSampleImpl<Sample<Matrix<T>>> BlockSample;
 		typedef BlockSampleImpl<const Sample<Matrix<T>>> ConstBlockSample;
 		typedef ScalarSampleImpl<Sample<Matrix<T>>> ScalarSample;
 		typedef ScalarSampleImpl<const Sample<Matrix<T>>> ConstScalarSample;

 	private:
 		unsigned int _nRow, _nCol;

 	public:
 		// Constructors
 		Sample();
 		explicit Sample(unsigned int size);
 		Sample(unsigned int size, unsigned int nRow, unsigned int nCol);
 		Sample(ConstBlockSample & blockSample);
 		Sample(ConstBlockSample && blockSample);
 		EIGEN_EXPR_CTOR(Sample, Sample<Matrix<T>>, Base, ArrayExpr)

 		// Destructor
 		virtual ~Sample() = default;

 		// Assignment operators
 		Sample& operator=(BlockSample & blockSample);
 		Sample& operator=(BlockSample && blockSample);
 		Sample& operator=(ConstBlockSample & blockSample);
 		Sample& operator=(ConstBlockSample && blockSample);

 		// Accessors
 		unsigned int size() const;
 		unsigned int rows() const;
 		unsigned int cols() const;
 		void resize(unsigned int size);
 		void resizeMatrix(unsigned int nRow, unsigned int nCol);
 		BlockSample block(index_t i, index_t j, unsigned int nRow, unsigned int nCol);
 		ConstBlockSample block(index_t i, index_t j, unsigned int nRow, unsigned int nCol) const;
 		BlockSample col(index_t j);
 		ConstBlockSample col(index_t j) const;
 		BlockSample row(index_t i);
 		ConstBlockSample row(index_t i) const;

 		// Operators
 		SampleType& operator[](unsigned int s);
 		const SampleType& operator[](unsigned int s) const;
 		ScalarSample operator()(index_t i, index_t j);
 		ConstScalarSample operator()(index_t i, index_t j) const;

 		// Statistics
 		SampleType mean(unsigned int begin = 0, int n = -1) const;
 		SampleType covariance(const Sample<Matrix<T>>& sample, unsigned int begin = 0, int n = -1) const;
 		auto covarianceMatrix(const Sample<Matrix<T>>& sample, unsigned int begin = 0, int n = -1) const
 			-> decltype(REDUX::tensorProd(
 				std::declval<Sample<Matrix<T>>>().segment(begin, n).redux(&REDUX::sum<SampleType>),
 				sample.segment(begin, n).redux(&REDUX::sum<SampleType>)));
 		SampleType variance(unsigned int begin = 0, int n = -1) const;
 		auto varianceMatrix(unsigned int begin = 0, int n = -1) const
 			-> decltype(std::declval<Sample<Matrix<T>>>().covarianceMatrix(std::declval<Sample<Matrix<T>>>(), begin, n));
 	};

 	template<typename T>
 	Sample<Matrix<T>>::Sample()
 	: Base()
 	, _nRow{0}
 	, _nCol{0}
 	{}

 	template<typename T>
 	Sample<Matrix<T>>::Sample(unsigned int size)
 	: Base(size)
 	, _nRow{0}
 	, _nCol{0}
 	{}

 	template<typename T>
 	Sample<Matrix<T>>::Sample(unsigned int size, unsigned int nRow, unsigned int nCol)
 	: Base(size)
 	{
 		resizeMatrix(nRow, nCol);
 	}

 	template<typename T>
 	Sample<Matrix<T>>::Sample(ConstBlockSample & blockSample)
 	: Sample(blockSample.size(), blockSample.rows(), blockSample.cols())
 	{
 		FOR_SAMPLE(*this, s)
 		{
 			(*this)[s] = blockSample[s];
 		}
 	}

 	template<typename T>
 	Sample<Matrix<T>>::Sample(ConstBlockSample && blockSample)
 	: Sample(blockSample.size(), blockSample.rows(), blockSample.cols())
 	{
 		FOR_SAMPLE(*this, s)
 		{
 			(*this)[s] = blockSample[s];
 		}
 	}

 	template<typename T>
 	Sample<Matrix<T>>& Sample<Matrix<T>>::operator=(BlockSample & blockSample)
 	{
 		Sample<Matrix<T>> tmp(blockSample);
 		this->swap(tmp);
 		return *this;
 	}
 	template<typename T>
 	Sample<Matrix<T>>& Sample<Matrix<T>>::operator=(BlockSample && blockSample)
 	{
 		*this = blockSample;
 		return *this;
 	}
 	template<typename T>
 	Sample<Matrix<T>>& Sample<Matrix<T>>::operator=(ConstBlockSample & blockSample)
 	{
 		Sample<Matrix<T>> tmp(blockSample);
 		this->swap(tmp);
 		return *this;
 	}
 	template<typename T>
 	Sample<Matrix<T>>& Sample<Matrix<T>>::operator=(ConstBlockSample && blockSample)
 	{
 		*this = blockSample;
 		return *this;
 	}

 	template<typename T>
 	unsigned int Sample<Matrix<T>>::size() const
 	{
 		return Base::size();
 	}
 	template<typename T>
 	unsigned int Sample<Matrix<T>>::rows() const
 	{
 		return _nRow;
 	}
 	template<typename T>
 	unsigned int Sample<Matrix<T>>::cols() const
 	{
 		return _nCol;
 	}

 	template<typename T>
 	void Sample<Matrix<T>>::resize(unsigned int size)
 	{
 		Base::resize(size);
 		_nRow = 0;
 		_nCol = 0;
 	}

 	template<typename T>
 	void Sample<Matrix<T>>::resizeMatrix(unsigned int nRow, unsigned int nCol)
 	{
 		FOR_SAMPLE(*this, s)
 		{
 			(*this)[s].resize(nRow, nCol);
 		}
 		_nRow = nRow;
 		_nCol = nCol;
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
 	typename Sample<Matrix<T>>::BlockSample Sample<Matrix<T>>::col(index_t j)
 	{
 		return Sample<Matrix<T>>::BlockSample(*this, 0, j, rows(), 1);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::ConstBlockSample Sample<Matrix<T>>::col(index_t j) const
 	{
 		return Sample<Matrix<T>>::ConstBlockSample(*this, 0, j, rows(), 1);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::BlockSample Sample<Matrix<T>>::row(index_t i)
 	{
 		return Sample<Matrix<T>>::BlockSample(*this, i, 0, 1, cols());
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::ConstBlockSample Sample<Matrix<T>>::row(index_t i) const
 	{
 		return Sample<Matrix<T>>::ConstBlockSample(*this, i, 0, 1, cols());
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::SampleType& Sample<Matrix<T>>::operator[](unsigned int s)
 	{
 		return Base::operator[](s);
 	}

 	template<typename T>
 	const typename Sample<Matrix<T>>::SampleType& Sample<Matrix<T>>::operator[](unsigned int s) const 
 	{
 		return Base::operator[](s);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::ScalarSample Sample<Matrix<T>>::operator()(index_t i, index_t j)
 	{
 		return Sample<Matrix<T>>::ScalarSample(*this, i, j);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::ConstScalarSample Sample<Matrix<T>>::operator()(index_t i, index_t j) const
 	{
 		return Sample<Matrix<T>>::ConstScalarSample(*this, i, j);
 	}


 	template<typename T>
 	typename Sample<Matrix<T>>::SampleType Sample<Matrix<T>>::mean(unsigned int begin, int n) const
 	{
 		SampleType result;
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			result = this->segment(begin, len).redux(&REDUX::sum<SampleType>);
 		}
 		return result / static_cast<double>(len);
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::SampleType Sample<Matrix<T>>::covariance(const Sample<Matrix<T>>& sample, unsigned int begin, int n) const
 	{
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			auto sampleV = sample.segment(begin, len);
 			auto thisV = this->segment(begin, len);

 			// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
 			return ( thisV.binaryExpr(sampleV, &REDUX::cwiseProd<SampleType>).redux(&REDUX::sum<SampleType>) 
 				- REDUX::cwiseProd(thisV.redux(&REDUX::sum<SampleType>), sampleV.redux(&REDUX::sum<SampleType>)) / static_cast<double>(len) )
 				/ static_cast<double>(len - 1);
 		}
 	}

 	template<typename T>
 	auto Sample<Matrix<T>>::covarianceMatrix(const Sample<Matrix<T>>& sample, unsigned int begin, int n) const
 	-> decltype(REDUX::tensorProd(
 		std::declval<Sample<Matrix<T>>>().segment(begin, n).redux(&REDUX::sum<SampleType>), 
 		sample.segment(begin, n).redux(&REDUX::sum<SampleType>)))
 	{
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			auto sampleV = sample.segment(begin, len);
 			auto thisV = this->segment(begin, len);

 			// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
 			return ( thisV.binaryExpr(sampleV, &REDUX::tensorProd<SampleType>).redux(&REDUX::sum<SampleType>) 
 				- REDUX::tensorProd(thisV.redux(&REDUX::sum<SampleType>), sampleV.redux(&REDUX::sum<SampleType>)) / static_cast<double>(len) )
 				/ static_cast<double>(len - 1);
 		}
 	}

 	template<typename T>
 	typename Sample<Matrix<T>>::SampleType Sample<Matrix<T>>::variance(unsigned int begin, int n) const
 	{
 		return covariance(*this, begin, n);
 	}

 	template<typename T>
 	auto Sample<Matrix<T>>::varianceMatrix(unsigned int begin, int n) const
 	-> decltype(std::declval<Sample<Matrix<T>>>().covarianceMatrix(std::declval<Sample<Matrix<T>>>(), begin, n))
	{
		return covarianceMatrix(*this, begin, n);
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
 	Sample<Matrix<T>>::BlockSampleImpl<S>::BlockSampleImpl(BlockSampleImpl<NonConstSType> & b)
 	: _Sample(b.sample())
 	, _i(b.startRow())
 	, _j(b.startCol())
 	, _nRow(b.rows())
 	, _nCol(b.cols())
 	{}
 	template<typename T>
	template<typename S>
 	Sample<Matrix<T>>::BlockSampleImpl<S>::BlockSampleImpl(BlockSampleImpl<NonConstSType> && b)
 	: BlockSampleImpl(b)
 	{}

 	template<typename T>
	template<typename S>
 	S& Sample<Matrix<T>>::BlockSampleImpl<S>::sample()
 	{
 		return _Sample;
 	}
 	template<typename T>
	template<typename S>
 	const S& Sample<Matrix<T>>::BlockSampleImpl<S>::sample() const
 	{
 		return _Sample;
 	}
 	template<typename T>
	template<typename S>
 	unsigned int Sample<Matrix<T>>::BlockSampleImpl<S>::size() const 
 	{
 		return _Sample.size();
 	}
 	template<typename T>
	template<typename S>
 	unsigned int Sample<Matrix<T>>::BlockSampleImpl<S>::rows() const 
 	{
 		return _nRow;
 	}
 	template<typename T>
	template<typename S>
 	unsigned int Sample<Matrix<T>>::BlockSampleImpl<S>::cols() const 
 	{
 		return _nCol;
 	}
 	template<typename T>
	template<typename S>
 	index_t Sample<Matrix<T>>::BlockSampleImpl<S>::startRow() const
 	{
 		return _i;
 	}
 	template<typename T>
	template<typename S>
 	index_t Sample<Matrix<T>>::BlockSampleImpl<S>::startCol() const
 	{
 		return _j;
 	}


 	template<typename T>
	template<typename S>
 	typename Sample<Matrix<T>>::template BlockSampleImpl<S>::BlockType
 	Sample<Matrix<T>>::BlockSampleImpl<S>::operator[](unsigned int s) 
 	{
 		return _Sample[s].block(_i, _j, _nRow, _nCol);
 	}

 	template<typename T>
	template<typename S>
 	typename Sample<Matrix<T>>::template BlockSampleImpl<S>::ConstBlockType 
 	Sample<Matrix<T>>::BlockSampleImpl<S>::operator[](unsigned int s) const
 	{
 		return _Sample[s].block(_i, _j, _nRow, _nCol);
 	}

 	template<typename T>
	template<typename S>
 	Sample<Matrix<T>>::ScalarSampleImpl<S>::ScalarSampleImpl(
 		S& s, 
 		const index_t i, const index_t j)
 	: _Sample(s)
 	, _i(i)
 	, _j(j)
 	{}

 	template<typename T>
	template<typename S>
 	unsigned int Sample<Matrix<T>>::ScalarSampleImpl<S>::size() const 
 	{
 		return _Sample.size();
 	}

 	template<typename T>
	template<typename S>
 	typename Sample<Matrix<T>>::template ScalarSampleImpl<S>::Scalar 
 	Sample<Matrix<T>>::ScalarSampleImpl<S>::operator[](unsigned int s) 
 	{
 		return _Sample[s](_i, _j);
 	}

 	template<typename T>
	template<typename S>
 	const typename Sample<Matrix<T>>::template ScalarSampleImpl<S>::Scalar 
 	Sample<Matrix<T>>::ScalarSampleImpl<S>::operator[](unsigned int s) const
 	{
 		return _Sample[s].block(_i, _j);
 	}

 }

#endif // MatrixExpr_SAMPLE_HPP