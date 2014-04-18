/*
 * BlockSample.hpp
 *
 *  Created on: Apr 11, 2014
 *      Author: Thibaut Metivet
 */

#ifndef BLOCK_SAMPLE_HPP
#define BLOCK_SAMPLE_HPP

 #include "Globals.hpp"
 #include "Sample.hpp"

 namespace LQCDA {

 	template<typename Derived>
 	class Sample<Block<Derived>>
 	{
 	protected:
 		// Typedefs
 		typedef Block<Derived> block_t;
 		typedef ConstBlock<Derived> const_block_t;
 	public:
 		typedef Block<Derived> sample_type;

 	private:
 		Sample<Derived>& _NestedSample;
 		index_t _i, _j, _nRow, _nCol;

 	public:
 		// Constructors
 		Sample(
 			Sample<Derived>& s, 
 			const index_t i, const index_t j, 
 			const index_t nRow, const index_t nCol);

 		// Destructor
 		virtual ~Sample() = default;

 		// Accessors
 		unsigned int size() const;

 		// Operators
 		block_t operator[](unsigned int s);
 		const_block_t operator[](unsigned int s) const;

 		// Statistics
 		Derived mean(unsigned int begin = 0, int n = -1) const;
 		Derived covariance(const Sample<Block<Derived>>& sample, unsigned int begin = 0, int n = -1) const;
 		auto covarianceMatrix(const Sample<Block<Derived>>& sample, unsigned int begin, int n) const
 			-> decltype(REDUX::tensorProd(
 				std::declval<Sample<Block<Derived>>>().segment(begin, n).redux(&REDUX::sum<sample_type>),
 				sample.segment(begin, n).redux(&REDUX::sum<sample_type>)));
 		Derived variance(unsigned int begin = 0, int n = -1) const;
 		auto varianceMatrix(unsigned int begin = 0, int n = -1) const
 			-> decltype(std::declval<Sample<Block<Derived>>>().covarianceMatrix(std::declval<Sample<Block<Derived>>>(), begin, n));
 	};

 	template<typename Derived>
 	Sample<Block<Derived>>::Sample(
 		Sample<Derived>& s, 
 		const index_t i, const index_t j, 
 		const index_t nRow, const index_t nCol)
 	: _NestedSample(s)
 	, _i(i)
 	, _j(j)
 	, _nRow(nRow)
 	, _nCol(nCol)
 	{}

 	template<typename Derived>
 	unsigned int Sample<Block<Derived>>::size() const
 	{
 		return _NestedSample.size();
 	}

 	template<typename Derived>
 	typename Sample<Block<Derived>>::block_t 
 	Sample<Block<Derived>>::operator[](unsigned int s)
 	{
 		return _NestedSample[s].block(_i, _j, _nRow, _nCol);
 	}

 	template<typename Derived>
 	typename Sample<Block<Derived>>::const_block_t 
 	Sample<Block<Derived>>::operator[](unsigned int s) const 
 	{
 		return _NestedSample[s].block(_i, _j, _nRow, _nCol);
 	}

 	template<typename Derived>
 	Derived Sample<Block<Derived>>::mean(unsigned int begin, int n) const
 	{
 		Derived result;
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			result = this->segment(begin, len).redux(&REDUX::sum<T>);
 		}
 		return result / static_cast<double>(len);
 	}

 	template<typename Derived>
 	Derived Sample<Block<Derived>>::covariance(const Sample<Block<Derived>>& sample, unsigned int begin, int n) const
 	{
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			auto sampleV = sample.segment(begin, len);
 			auto thisV = this->segment(begin, len);

 			// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
 			return ( thisV.binaryExpr(sampleV, &REDUX::prod<T>).redux(&REDUX::sum<T>) 
 				- REDUX::prod(thisV.redux(&REDUX::sum<T>), sampleV.redux(&REDUX::sum<T>)) / static_cast<double>(len) )
 				/ static_cast<double>(len - 1);
 		}
 	}

 	template<typename Derived>
 	auto Sample<Block<Derived>>::covarianceMatrix(const Sample<Block<Derived>>& sample, unsigned int begin, int n) const
 	-> decltype(REDUX::tensorProd(std::declval<Sample<Block<Derived>>>().segment(begin, n).redux(&REDUX::sum<T>), sample.segment(begin, n).redux(&REDUX::sum<T>)))
 	{
 		const unsigned int len = (n >= 0)? n: size();
 		if(len)
 		{
 			auto sampleV = sample.segment(begin, len);
 			auto thisV = this->segment(begin, len);

 			// COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
 			return ( thisV.binaryExpr(sampleV, &REDUX::tensorProd<T>).redux(&REDUX::sum<T>) 
 				- REDUX::tensorProd(thisV.redux(&REDUX::sum<T>), sampleV.redux(&REDUX::sum<T>)) / static_cast<double>(len) )
 				/ static_cast<double>(len - 1);
 		}
 	}

 	template<typename Derived>
 	T Sample<Block<Derived>>::variance(unsigned int begin, int n) const
 	{
 		return covariance(*this, begin, n);
 	}

 	template<typename Derived>
 	auto Sample<Block<Derived>>::varianceMatrix(unsigned int begin, int n) const
 	-> decltype(std::declval<Sample<Block<Derived>>>().covarianceMatrix(std::declval<Sample<Block<Derived>>>(), begin, n))
	{
		return covarianceMatrix(*this, begin, n);
	}

 }

#endif // BLOCK_SAMPLE_HPP