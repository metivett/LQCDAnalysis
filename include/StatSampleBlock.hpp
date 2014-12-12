/*
 * StatSampleBlock.hpp
 *
 *  Created on: Dec 11, 2014
 *      Author: Thibaut Metivet
 */

#ifndef STAT_SAMPLE_BLOCK_HPP
#define STAT_SAMPLE_BLOCK_HPP

#include "Globals.hpp"
#include "Math.hpp"
#include "StatSampleBase.hpp"
#include "Reduction.hpp"
#include "Statistics.hpp"

BEGIN_NAMESPACE(LQCDA)

template<typename S, int BlockRows=Dynamic, int BlockCols=Dynamic>
class StatSampleBlock;

BEGIN_NAMESPACE(internal)

template<typename S, int BlockRows, int BlockCols>
struct sample_traits<StatSampleBlock<S, BlockRows, BlockCols>>
{
    typedef typename internal::sample_traits<S>::SampleElement SampleElement;
    typedef Block<SampleElement, BlockRows, BlockCols> SampleElementRef;
    typedef Block<const SampleElement, BlockRows, BlockCols> ConstSampleElementRef;
    typedef typename internal::sample_traits<S>::ScalarType ScalarType;
};

END_NAMESPACE // internal

template<typename S, int BlockRows, int BlockCols>
class StatSampleBlock
    : public StatSampleBase<StatSampleBlock<S, BlockRows, BlockCols>>
{
public: // Typedefs
    typedef StatSampleBlock<S, BlockRows, BlockCols> This;
    LQCDA_SAMPLE_TRAITS_TYPEDEFS(This)
    typedef typename std::remove_const<S>::type NonConstSType;

protected: // Data
    const S &m_Sample;
    index_t m_startRow, m_startCol;
    index_t m_nRow, m_nCol;

public: // Constructors and Destructor
    // Fixed-size constructor
    StatSampleBlock(
        const S &s,
        const index_t i, const index_t j)
        : m_Sample(s)
        , m_startRow(i)
        , m_startCol(j)
        , m_nRow(BlockRows)
        , m_nCol(BlockCols)
    {}
    // Dynamic-size constructor
    StatSampleBlock(
        const S &s,
        const index_t i, const index_t j,
        const index_t nRow, const index_t nCol)
        : m_Sample(s)
        , m_startRow(i)
        , m_startCol(j)
        , m_nRow(nRow)
        , m_nCol(nCol)
    {}
    // Copy constructors
    StatSampleBlock(StatSampleBlock<NonConstSType, BlockRows, BlockCols> &b)
    : m_Sample(b.nestedSample())
    , m_startRow(b.startRow())
    , m_startCol(b.startCol())
    , m_nRow(b.rows())
    , m_nCol(b.cols())
    {}
    StatSampleBlock(StatSampleBlock<NonConstSType, BlockRows, BlockCols> &&b)
    : StatSampleBlock(b)
    {}
    // Destructor
    ~StatSampleBlock() = default;

public: // Assignment
    StatSampleBlock &operator=(const S &sample)
    {
        for (unsigned int s = 0; s < m_Sample.size(); ++s)
        {
            m_Sample[s].block(m_startRow, m_startCol, m_nRow, m_nCol) = sample[s];
        }
        return *this;
    }
    StatSampleBlock &operator=(const S && sample)
    {
        *this = sample;
        return *this;
    }

public: // Queries
    unsigned int size() const
    {
        return m_Sample.size();
    }
    unsigned int rows() const
    {
        return m_nRow;
    }
    unsigned int cols() const
    {
        return m_nCol;
    }
    index_t startRow() const
    {
        return m_startRow;
    }
    index_t startCol() const
    {
        return m_startCol;
    }

public: // Subscript
    SampleElementRef operator[](unsigned int s)
    {
        // return Block<SampleElement, BlockRows, BlockCols>(m_Sample[s], m_startRow, m_startCol, m_nRow, m_nCol);
        return m_Sample[s].template block<BlockRows, BlockCols>(m_startRow, m_startCol, m_nRow, m_nCol);
    }
    ConstSampleElementRef operator[](unsigned int s) const
    {
        // return Block<SampleElement, BlockRows, BlockCols>(m_Sample[s], m_startRow, m_startCol, m_nRow, m_nCol);
        return m_Sample[s].template block<BlockRows, BlockCols>(m_startRow, m_startCol, m_nRow, m_nCol);
    }

public: // Utilities
    const typename std::decay<S>::type& nestedSample() const
    {
        return m_Sample;
    }

public: // Statistics
    SampleElement mean() const
    {
        SampleElement res = (*this)[0];
        for(int s = 1; s < size(); ++s)
            res = res + (*this)[s];

        return res / static_cast<double>(size());
    }
    SampleElement median() const
    {
        SampleElement res(rows(), cols());
        std::vector<ScalarType> vtmp(size());
        FOR_MAT(res, i, j)
        {
            for (int s = 0; s < size(); ++s)
                vtmp[s] = m_Sample[s](i, j);
            res(i, j) = internal::direct_median(vtmp);
        }
        return res;
    }
    SampleElement variance() const
    {
        return covariance(*this);
    }
    template<typename OtherDerived>
    SampleElement covariance(const StatSampleBase<OtherDerived> &other) const
    {
        if ((this->rows() != other.rows()) || (this->cols() != other.cols()))
        {
            ERROR(SIZE, "covariance called with Samples of different dimensions");
        }
        if (this->size() != other.size())
        {
            ERROR(SIZE, "covariance called with Samples of different sizes");
        }

        // COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
        SampleElement sxiyi = REDUX::cwiseProd<SampleElement>((*this)[0], other[0]);
        SampleElement sxi = (*this)[0];
        SampleElement syi = other[0];
        for(int s = 1; s < size(); ++s)
        {
            sxiyi = sxiyi + REDUX::cwiseProd<SampleElement>((*this)[s], other[s]);
            sxi = sxi + (*this)[s];
            syi = syi + other[s];
        }

        return ( sxiyi - REDUX::cwiseProd(sxi, syi) / static_cast<double>(size()) ) / static_cast<double>(size() - 1);
    }
    Matrix<SampleElement> varianceMatrix() const
    {
        return covarianceMatrix(*this);
    }
    template<typename OtherDerived>
    Matrix<SampleElement> covarianceMatrix(const StatSampleBase<OtherDerived> &other) const
    {
        if ((this->cols() != 1) || (other.cols() != 1))
        {
            ERROR(SIZE, "tensor product only valid for column vectors");
        }

        auto blockSum = [this](const SampleElement & a, const SampleElement & b)
        {
            SampleElement res(a.rows(), a.cols());
            res.block(m_startRow, m_startCol, m_nRow, m_nCol) = a.block(m_startRow, m_startCol, m_nRow, m_nCol) + b.block(m_startRow, m_startCol, m_nRow, m_nCol);
            return res;
        };

        auto blockTensorProd = [this, other](const SampleElement & a, const SampleElement & b)
        {
            SampleElement res(rows(), other.rows());
            res = a.block(m_startRow, m_startCol, m_nRow, m_nCol) * b.block(other.startRow(), other.startCol(), other.rows(), other.cols()).transpose();
            return res;
        };

        // COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
        return ( m_Sample.binaryExpr(other.m_Sample, blockTensorProd).redux(&REDUX::sum<SampleElement>)
                 - blockTensorProd(m_Sample.redux(blockSum), other.m_Sample.redux(blockSum)) / static_cast<double>(size()) )
               / static_cast<double>(size() - 1);
    }

    SampleElement standardDeviation() const
    {
        using Eigen::sqrt;
        using std::sqrt;
        return sqrt(this->variance());
    }
    SampleElement medianDeviation() const
    {
        using Eigen::abs;
        using std::abs;

        SampleElement res(rows(), cols());
        SampleElement med = this->median();
        std::vector<ScalarType> vtmp(size());
        FOR_MAT(res, i, j)
        {
            for (int s = 0; s < size(); ++s)
                vtmp[s] = abs(m_Sample[s](i, j) - med(i, j));
            res(i, j) = internal::direct_median(vtmp);
        }
        return res;
    }

};

END_NAMESPACE // LQCDA

#endif // STAT_SAMPLE_BLOCK_HPP
