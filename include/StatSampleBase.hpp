/*
 * StatSampleBase.hpp
 *
 *  Created on: Dec 09, 2014
 *      Author: Thibaut Metivet
 */

#ifndef STAT_SAMPLE_BASE_HPP
#define STAT_SAMPLE_BASE_HPP

#include "Globals.hpp"
#include "TypeTraits.hpp"

BEGIN_NAMESPACE(LQCDA)

BEGIN_NAMESPACE(internal)

template<typename T> struct sample_traits;

END_NAMESPACE // internal

#define LQCDA_SAMPLE_TRAITS_TYPEDEFS(type) \
typedef typename internal::sample_traits<type>::SampleElement SampleElement; \
typedef typename internal::sample_traits<type>::SampleElementRef SampleElementRef; \
typedef typename internal::sample_traits<type>::ConstSampleElementRef ConstSampleElementRef; \
typedef typename internal::sample_traits<type>::ScalarType ScalarType;

template<typename Derived>
class StatSampleBase
{
public: // Typedefs
    LQCDA_SAMPLE_TRAITS_TYPEDEFS(Derived)

public: // Access to derived object
    Derived& derived() { return *static_cast<Derived*>(this); }
    const Derived& derived() const { return *static_cast<const Derived*>(this); }

public: // Queries
    unsigned int size() const
    {
        return derived().size();
    }
    unsigned int rows() const
    {
        return derived().rows();
    }
    unsigned int cols() const
    {
        return derived().cols();
    }
    index_t startRow() const
    {
        return derived().startRow();
    }
    index_t startCol() const
    {
        return derived().startCol();
    }

public: // Subscript
    SampleElementRef operator[](unsigned int s)
    {
        return derived().operator[](s);
    }
    ConstSampleElementRef operator[](unsigned int s) const
    {
        return derived().operator[](s);
    }

public: // Statistics
    SampleElement mean() const { return derived().mean(); }
    SampleElement median() const { return derived().median(); }
    SampleElement variance() const { return derived().variance(); }
    SampleElement covariance(const StatSampleBase<Derived> &other) const { return this->derived().covariance(other.derived()); }
    Matrix<SampleElement> varianceMatrix() const { return derived().varianceMatrix(); }
    Matrix<SampleElement> covarianceMatrix(const StatSampleBase<Derived> &other) const { return this->derived().covarianceMatrix(other.derived()); }
    SampleElement standardDeviation() const { return derived().standardDeviation(); }
    SampleElement medianDeviation() const { return derived().medianDeviation(); }

};



END_NAMESPACE // LQCDA

#endif // STAT_SAMPLE_BASE_HPP
