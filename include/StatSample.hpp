/*
 * StatSample.hpp
 *
 *  Created on: Nov 04, 2014
 *      Author: Thibaut Metivet
 */

#ifndef STAT_SAMPLE_HPP
#define STAT_SAMPLE_HPP

#include "Globals.hpp"
#include "Reduction.hpp"
#include "Statistics.hpp"

BEGIN_NAMESPACE(LQCDA)

#define USING_STATSAMPLE_STATS(S) \
using S::mean; \
using S::median; \
using S::variance; \
using S::covariance; \
using S::varianceMatrix; \
using S::covarianceMatrix;

BEGIN_NAMESPACE(internal)

template<typename T> struct sample_traits;

END_NAMESPACE // internal

template<typename Derived>
class StatSample
{
public:
    typedef typename internal::sample_traits<Derived>::SampleElement SampleElement;

protected:
    Array<SampleElement, Dynamic, 1> m_Sample;

public:
    // Constructors
    StatSample()
        : m_Sample()
    {}
    explicit StatSample(unsigned int size)
        : m_Sample(size)
    {}
    template<typename OtherDerived>
    StatSample(const StatSample<OtherDerived> &other)
        : m_Sample(other.m_Sample)
    {}

    // Assignment
    template<typename OtherDerived>
    Derived &operator=(const StatSample<OtherDerived> &other)
    {
        m_Sample = other.m_Sample;
    }

    // Destructor
    virtual ~StatSample() = default;

    // Queries
    unsigned int size() const
    {
        return m_Sample.size();
    }

    // Resize
    void resize(unsigned int size)
    {
        m_Sample.resize(size);
    }

    // Subscript
    SampleElement &operator[](unsigned int s)
    {
        return m_Sample[s];
    }
    const SampleElement &operator[](unsigned int s) const
    {
        return m_Sample[s];
    }

    // Utilities
    template<typename OtherDerived>
    void swap(const StatSample<OtherDerived>& other)
    {
        m_Sample.swap(other.m_Sample);
    }

    // Statistics
    SampleElement mean() const
    {
        return m_Sample.redux(&REDUX::sum<SampleElement>) / static_cast<double>(size());
    }
    SampleElement median() const
    {
        return LQCDA::median(m_Sample.data(), m_Sample.data()+m_Sample.size());
    }
    SampleElement variance() const
    {
        return covariance(*this);
    }
    SampleElement covariance(const StatSample<Derived> &other) const
    {
        // COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
        return ( m_Sample.binaryExpr(other.m_Sample, &REDUX::cwiseProd<SampleElement>).redux(&REDUX::sum<SampleElement>)
                 - REDUX::cwiseProd(m_Sample.redux(&REDUX::sum<SampleElement>), other.m_Sample.redux(&REDUX::sum<SampleElement>)) / static_cast<double>(size()) )
               / static_cast<double>(size() - 1);
    }
    Matrix<SampleElement> varianceMatrix() const
    {
        return covarianceMatrix(*this);
    }
    Matrix<SampleElement> covarianceMatrix(const StatSample<Derived> &other) const
    {
        // COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
        return ( m_Sample.binaryExpr(other.m_Sample, &REDUX::tensorProd<SampleElement>).redux(&REDUX::sum<SampleElement>)
                 - REDUX::tensorProd(m_Sample.redux(&REDUX::sum<SampleElement>), other.m_Sample.redux(&REDUX::sum<SampleElement>)) / static_cast<double>(size()) )
               / static_cast<double>(size() - 1);
    }
};

END_NAMESPACE // LQCDA

#endif // STAT_SAMPLE_HPP
