/*
 * MatrixSample.hpp
 *
 *  Created on: Apr 11, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MATRIX_SAMPLE_HPP
#define MATRIX_SAMPLE_HPP

#include "Globals.hpp"
#include "TypeTraits.hpp"
#include "Sample.hpp"
#include "StatSampleBlock.hpp"

#include <functional>

BEGIN_NAMESPACE(LQCDA)

BEGIN_NAMESPACE(internal)

template<typename T>
struct sample_traits<Sample<Matrix<T>>>
{
    typedef Matrix<T> SampleElement;
    typedef Matrix<T>& SampleElementRef;
    typedef const Matrix<T>& ConstSampleElementRef;
    typedef T ScalarType;
};

END_NAMESPACE // internal

template<typename T>
class Sample<Matrix<T>>
: public StatSample<Sample<Matrix<T>>>
{
private: // BlockSampleImpl
    // template<typename S>
    // class BlockSampleImpl
    // {
    // private:
    //     // Typedefs
    //     typedef typename
    //     if_<std::is_const<S>::value,
    //         const typename S::SampleElement, typename S::SampleElement>::result SampleElement;
    //     typedef typename std::remove_const<S>::type NonConstSType;
    //     typedef Block<SampleElement> BlockType;
    //     typedef ConstBlock<SampleElement> ConstBlockType;

    // private:
    //     // Data
    //     S &_Sample;
    //     index_t _i, _j, _nRow, _nCol;

    // public:
    //     // Constructors
    //     BlockSampleImpl(
    //         S &s,
    //         const index_t i, const index_t j,
    //         const index_t nRow, const index_t nCol);
    //     BlockSampleImpl(BlockSampleImpl<NonConstSType> &b);
    //     BlockSampleImpl(BlockSampleImpl<NonConstSType>  &&b);
    //     // Destructor
    //     ~BlockSampleImpl() = default;
    //     // Assignment
    //     BlockSampleImpl<S> &operator=(const S &sample);
    //     BlockSampleImpl<S> &operator=(const S && sample);

    //     // Accessors
    //     S &sample();
    //     const S &sample() const;
    //     unsigned int size() const;
    //     unsigned int rows() const;
    //     unsigned int cols() const;
    //     index_t startRow() const;
    //     index_t startCol() const;

    //     // Operators
    //     BlockType operator[](unsigned int s);
    //     ConstBlockType operator[](unsigned int s) const;

    //     // Statistics
    //     SampleElement mean(unsigned int begin = 0, int n = -1) const;
    //     SampleElement covariance(const Sample<Matrix<T>> &sample, unsigned int begin = 0, int n = -1) const;
    //     auto covarianceMatrix(const Sample<Matrix<T>> &sample, unsigned int begin = 0, int n = -1) const
    //     -> decltype(REDUX::tensorProd(
    //                     std::declval<Sample<Matrix<T>>>().segment(begin, n).redux(&REDUX::sum<SampleElement>),
    //                     sample.segment(begin, n).redux(&REDUX::sum<SampleElement>)));
    //     SampleElement variance(unsigned int begin = 0, int n = -1) const;
    //     auto varianceMatrix(unsigned int begin = 0, int n = -1) const
    //     -> decltype(std::declval<Sample<Matrix<T>>>().covarianceMatrix(std::declval<Sample<Matrix<T>>>(), begin, n));

    // };

private: // ScalarSampleImpl
    // template<typename S>
    // class ScalarSampleImpl
    // {
    // private:
    //     // Typedefs
    //     typedef typename S::Scalar Scalar;
    //     typedef typename
    //     if_<std::is_const<S>::value,
    //         const typename S::SampleElement, typename S::SampleElement>::result SampleElement;
    //     typedef typename std::remove_const<S>::type NonConstSType;

    // private:
    //     // Data
    //     S &_Sample;
    //     index_t _i, _j;

    // public:
    //     // Constructors
    //     ScalarSampleImpl(
    //         S &s,
    //         const index_t i, const index_t j);
    //     ScalarSampleImpl(ScalarSampleImpl<NonConstSType> &b);
    //     ScalarSampleImpl(ScalarSampleImpl<NonConstSType>  &&b);
    //     // Destructor
    //     ~ScalarSampleImpl() = default;
    //     // Assignment
    //     ScalarSampleImpl<S> &operator=(const S &sample);
    //     ScalarSampleImpl<S> &operator=(const S && sample);
    //     ScalarSampleImpl<S> &operator=(const Sample<Scalar> &sample);
    //     ScalarSampleImpl<S> &operator=(const Sample<Scalar> && sample);

    //     // Accessors
    //     S &sample();
    //     const S &sample() const;
    //     unsigned int size() const;

    //     // Operators
    //     Scalar operator[](unsigned int s);
    //     const Scalar operator[](unsigned int s) const;

    // };

public: // Typedefs
    typedef StatSample < Sample<Matrix<T> >> Base;
    typedef Sample<Matrix<T>> This;
    LQCDA_SAMPLE_TRAITS_TYPEDEFS(This)

public:
    typedef StatSampleBlock<Sample<Matrix<T>>> BlockSample;
    typedef const StatSampleBlock<const Sample<Matrix<T>>> ConstBlockSample;
    typedef StatSampleBlock<Sample<Matrix<T>>, 1, 1> ScalarSample;
    typedef const StatSampleBlock<const Sample<Matrix<T>>, 1, 1> ConstScalarSample;

private:
    // unsigned int _nRow, _nCol;
    static inline Matrix<T> scalarMul(const Matrix<T> &m, const T &t)
    {
        return m * t;
    }
    static inline Matrix<T> scalarDiv(const Matrix<T> &m, const T &t)
    {
        return m / t;
    }

public:
    // Constructors
    Sample();
    explicit Sample(unsigned int size);
    Sample(unsigned int size, unsigned int nRow, unsigned int nCol);
    Sample(ConstBlockSample &blockSample);
    Sample(ConstBlockSample &&blockSample);

    // Sample(const Sample<Matrix<T>>& e): Base(e), _nRow(e._nRow), _nCol(e._nCol) {}
    // Sample<Matrix<T>>& operator=(const Sample<Matrix<T>>& e)
    // {
    //  this->Base::operator=(e);
    //  return *this;
    // }

    // Destructor
    virtual ~Sample() = default;

    // Assignment operators
    Sample &operator=(BlockSample &blockSample);
    Sample &operator=(BlockSample && blockSample);
    Sample &operator=(ConstBlockSample &blockSample);
    Sample &operator=(ConstBlockSample && blockSample);

    // Queries
    unsigned int rows() const;
    unsigned int cols() const;
    void resizeMatrix(unsigned int nRow, unsigned int nCol);
    void conservativeResizeMatrix(unsigned int nRow, unsigned int nCol);
    BlockSample block(index_t i, index_t j, unsigned int nRow, unsigned int nCol);
    ConstBlockSample block(index_t i, index_t j, unsigned int nRow, unsigned int nCol) const;
    BlockSample col(index_t j);
    ConstBlockSample col(index_t j) const;
    BlockSample row(index_t i);
    ConstBlockSample row(index_t i) const;

    // Operators
    SampleElement &operator[](unsigned int s);
    const SampleElement &operator[](unsigned int s) const;
    ScalarSample operator()(index_t i, index_t j);
    ConstScalarSample operator()(index_t i, index_t j) const;
    // using Base::operator+=;
    // Sample<Matrix<T>> &operator+=(const T &t);
    // Sample<Matrix<T>> &operator+=(const T && t);
    // using Base::operator-=;
    // Sample<Matrix<T>> &operator-=(const T &t);
    // Sample<Matrix<T>> &operator-=(const T && t);
    // using Base::operator*=;
    // Sample<Matrix<T>> &operator*=(const T &t);
    // Sample<Matrix<T>> &operator*=(const T && t);
    // using Base::operator/=;
    // Sample<Matrix<T>> &operator/=(const T &t);
    // Sample<Matrix<T>> &operator/=(const T && t);
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
Sample<Matrix<T>>::Sample(unsigned int size, unsigned int nRow, unsigned int nCol)
                   : Base(size)
{
    resizeMatrix(nRow, nCol);
}

template<typename T>
Sample<Matrix<T>>::Sample(ConstBlockSample &blockSample)
                   : Sample(blockSample.size(), blockSample.rows(), blockSample.cols())
{
    FOR_SAMPLE(*this, s)
    {
        (*this)[s] = blockSample[s];
    }
}

template<typename T>
Sample<Matrix<T>>::Sample(ConstBlockSample  &&blockSample)
                   : Sample(blockSample.size(), blockSample.rows(), blockSample.cols())
{
    FOR_SAMPLE(*this, s)
    {
        (*this)[s] = blockSample[s];
    }
}

template<typename T>
Sample<Matrix<T>> &Sample<Matrix<T>>::operator=(BlockSample &blockSample)
{
    Sample<Matrix<T>> tmp(blockSample);
    this->swap(tmp);
    return *this;
}
template<typename T>
Sample<Matrix<T>> &Sample<Matrix<T>>::operator=(BlockSample && blockSample)
{
    *this = blockSample;
    return *this;
}
template<typename T>
Sample<Matrix<T>> &Sample<Matrix<T>>::operator=(ConstBlockSample &blockSample)
{
    Sample<Matrix<T>> tmp(blockSample);
    this->swap(tmp);
    return *this;
}
template<typename T>
Sample<Matrix<T>> &Sample<Matrix<T>>::operator=(ConstBlockSample && blockSample)
{
    *this = blockSample;
    return *this;
}

template<typename T>
unsigned int Sample<Matrix<T>>::rows() const
{
    return Base::operator[](0).rows();
}
template<typename T>
unsigned int Sample<Matrix<T>>::cols() const
{
    return Base::operator[](0).cols();
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
void Sample<Matrix<T>>::conservativeResizeMatrix(unsigned int nRow, unsigned int nCol)
{
    FOR_SAMPLE(*this, s)
    {
        (*this)[s].conservativeResize(nRow, nCol);
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
typename Sample<Matrix<T>>::SampleElement &Sample<Matrix<T>>::operator[](unsigned int s)
{
    return Base::operator[](s);
}

template<typename T>
const typename Sample<Matrix<T>>::SampleElement &Sample<Matrix<T>>::operator[](unsigned int s) const
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

// template<typename T>
// Sample<Matrix<T>> &Sample<Matrix<T>>::operator+=(const T &t)
// {
//     this->unaryExpr([&t](const Matrix<T> &m)
//     {
//         return m + t;
//     });
//     return *this;
// }

// template<typename T>
// Sample<Matrix<T>> &Sample<Matrix<T>>::operator+=(const T && t)
// {
//     return (*this) += t;
// }

// template<typename T>
// Sample<Matrix<T>> &Sample<Matrix<T>>::operator-=(const T &t)
// {
//     Base::operator-=(Matrix<T>::Constant(rows(), cols(), t));
//     return *this;
// }

// template<typename T>
// Sample<Matrix<T>> &Sample<Matrix<T>>::operator-=(const T && t)
// {
//     return (*this) -= t;
// }

// template<typename T>
// Sample<Matrix<T>> &Sample<Matrix<T>>::operator*=(const T &t)
// {
//     Base::operator*=(Matrix<T>::Constant(rows(), cols(), t));
//     return *this;
// }

// template<typename T>
// Sample<Matrix<T>> &Sample<Matrix<T>>::operator*=(const T && t)
// {
//     return (*this) *= t;
// }

// template<typename T>
// Sample<Matrix<T>> &Sample<Matrix<T>>::operator/=(const T &t)
// {
//     return (*this) *=  1 / t;
// }

// template<typename T>
// Sample<Matrix<T>> &Sample<Matrix<T>>::operator/=(const T && t)
// {
//     return (*this) /= t;
// }




// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::BlockSampleImpl<S>::BlockSampleImpl(
//                    S &s,
//                    const index_t i, const index_t j,
//                    const index_t nRow, const index_t nCol)
//                    : _Sample(s)
//                    , _i(i)
//                    , _j(j)
//                    , _nRow(nRow)
//                    , _nCol(nCol)
// {}
// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::BlockSampleImpl<S>::BlockSampleImpl(BlockSampleImpl<NonConstSType> &b)
//                    : _Sample(b.sample())
//                    , _i(b.startRow())
//                    , _j(b.startCol())
//                    , _nRow(b.rows())
//                    , _nCol(b.cols())
// {}
// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::BlockSampleImpl<S>::BlockSampleImpl(BlockSampleImpl<NonConstSType>  &&b)
//                    : BlockSampleImpl(b)
// {}

// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::BlockSampleImpl<S> &Sample<Matrix<T>>::BlockSampleImpl<S>::operator=(const S &sample)
// {
//     FOR_SAMPLE(_Sample, s)
//     {
//         _Sample[s].block(_i, _j, _nRow, _nCol) = sample[s];
//     }

//     return *this;
// }
// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::BlockSampleImpl<S> &Sample<Matrix<T>>::BlockSampleImpl<S>::operator=(const S && sample)
// {
//     *this = sample;
//     return *this;
// }

// template<typename T>
// template<typename S>
// S &Sample<Matrix<T>>::BlockSampleImpl<S>::sample()
// {
//     return _Sample;
// }
// template<typename T>
// template<typename S>
// const S &Sample<Matrix<T>>::BlockSampleImpl<S>::sample() const
// {
//     return _Sample;
// }
// template<typename T>
// template<typename S>
// unsigned int Sample<Matrix<T>>::BlockSampleImpl<S>::size() const
// {
//     return _Sample.size();
// }
// template<typename T>
// template<typename S>
// unsigned int Sample<Matrix<T>>::BlockSampleImpl<S>::rows() const
// {
//     return _nRow;
// }
// template<typename T>
// template<typename S>
// unsigned int Sample<Matrix<T>>::BlockSampleImpl<S>::cols() const
// {
//     return _nCol;
// }
// template<typename T>
// template<typename S>
// index_t Sample<Matrix<T>>::BlockSampleImpl<S>::startRow() const
// {
//     return _i;
// }
// template<typename T>
// template<typename S>
// index_t Sample<Matrix<T>>::BlockSampleImpl<S>::startCol() const
// {
//     return _j;
// }


// template<typename T>
// template<typename S>
// typename Sample<Matrix<T>>::template BlockSampleImpl<S>::BlockType
// Sample<Matrix<T>>::BlockSampleImpl<S>::operator[](unsigned int s)
// {
//     return _Sample[s].block(_i, _j, _nRow, _nCol);
// }

// template<typename T>
// template<typename S>
// typename Sample<Matrix<T>>::template BlockSampleImpl<S>::ConstBlockType
// Sample<Matrix<T>>::BlockSampleImpl<S>::operator[](unsigned int s) const
// {
//     return _Sample[s].block(_i, _j, _nRow, _nCol);
// }

// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::ScalarSampleImpl<S>::ScalarSampleImpl(
//                    S &s,
//                    const index_t i, const index_t j)
//                    : _Sample(s)
//                    , _i(i)
//                    , _j(j)
// {}
// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::ScalarSampleImpl<S>::ScalarSampleImpl(ScalarSampleImpl<NonConstSType> &b)
//                    : _Sample(b.sample())
//                    , _i(b._i)
//                    , _j(b._j)
// {}
// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::ScalarSampleImpl<S>::ScalarSampleImpl(ScalarSampleImpl<NonConstSType>  &&b)
//                    : ScalarSampleImpl(b)
// {}

// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::ScalarSampleImpl<S> &Sample<Matrix<T>>::ScalarSampleImpl<S>::operator=(const S &sample)
// {
//     FOR_SAMPLE(_Sample, s)
//     {
//         _Sample[s].block(_i, _j, 1, 1) = sample[s];
//     }

//     return *this;
// }
// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::ScalarSampleImpl<S> &Sample<Matrix<T>>::ScalarSampleImpl<S>::operator=(const S && sample)
// {
//     *this = sample;
//     return *this;
// }
// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::ScalarSampleImpl<S> &Sample<Matrix<T>>::ScalarSampleImpl<S>::operator=(const Sample<Scalar> &sample)
// {
//     FOR_SAMPLE(_Sample, s)
//     {
//         _Sample[s](_i, _j) = sample[s];
//     }

//     return *this;
// }
// template<typename T>
// template<typename S>
// Sample<Matrix<T>>::ScalarSampleImpl<S> &Sample<Matrix<T>>::ScalarSampleImpl<S>::operator=(const Sample<Scalar> && sample)
// {
//     *this = sample;
//     return *this;
// }

// template<typename T>
// template<typename S>
// S &Sample<Matrix<T>>::ScalarSampleImpl<S>::sample()
// {
//     return _Sample;
// }
// template<typename T>
// template<typename S>
// const S &Sample<Matrix<T>>::ScalarSampleImpl<S>::sample() const
// {
//     return _Sample;
// }
// template<typename T>
// template<typename S>
// unsigned int Sample<Matrix<T>>::ScalarSampleImpl<S>::size() const
// {
//     return _Sample.size();
// }

// template<typename T>
// template<typename S>
// typename Sample<Matrix<T>>::template ScalarSampleImpl<S>::Scalar
// Sample<Matrix<T>>::ScalarSampleImpl<S>::operator[](unsigned int s)
// {
//     return _Sample[s](_i, _j);
// }

// template<typename T>
// template<typename S>
// const typename Sample<Matrix<T>>::template ScalarSampleImpl<S>::Scalar
// Sample<Matrix<T>>::ScalarSampleImpl<S>::operator[](unsigned int s) const
// {
//     return _Sample[s].block(_i, _j);
// }

END_NAMESPACE // LQCDA

#endif // MatrixExpr_SAMPLE_HPP