/*
 * Statistics.hpp
 *
 *  Created on: Apr 24, 2014
 *      Author: Thibaut Metivet
 */

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "Globals.hpp"
#include "Reduction.hpp"
#include "TypeTraits.hpp"

BEGIN_NAMESPACE(LQCDA)

template<typename InputIterator>
typename InputIterator::value_type mean(InputIterator first, InputIterator last)
{
    using ValueType = typename InputIterator::value_type;
    return reduce(first, last, &REDUX::sum<ValueType>) / static_cast<double>(std::distance(first, last));
}

BEGIN_NAMESPACE(internal)

template <
    typename T,
    typename = typename std::enable_if<std::is_floating_point<T>::value>::type
    >
T direct_median(std::vector<T> &v)
{
    // performs in-place nth_element
    auto len = v.size();
    unsigned int half = len / 2;
    std::nth_element(v.begin(), v.begin() + half, v.end());
    if (len % 2 == 0)
    {
        return (*std::max_element(v.begin(), v.begin() + half) + v[half]) / static_cast<T>(2);
    }
    else
    {
        return v[len / 2];
    }
}

END_NAMESPACE // internal

template <typename RAIter>
typename std::enable_if <
supports_less_than<typename std::iterator_traits<RAIter>::value_type>
::value,
typename std::iterator_traits<RAIter>::value_type >::type
median(RAIter first, RAIter last)
{
    using ValueType = typename std::iterator_traits<RAIter>::value_type;
    // performs out-of-place median search
    std::vector<ValueType> vcpy(first, last);
    return internal::direct_median(vcpy);
}

template <typename RAIter>
typename std::enable_if <
std::is_base_of<DenseExpr<typename std::iterator_traits<RAIter>::value_type>, typename std::iterator_traits<RAIter>::value_type>
::value,
typename std::iterator_traits<RAIter>::value_type >::type
median(RAIter first, RAIter last)
{
    using ValueType = typename std::iterator_traits<RAIter>::value_type;
    using ScalarType = typename std::remove_reference<decltype(std::declval<ValueType>()[0])>::type;
    // performs out-of-place median search
    std::vector<ScalarType> vtmp(std::distance(first, last));
    ValueType med = *first;
    unsigned int size = med.size();

    for (int index = 0; index < size; ++index)
    {
        int i = 0;
        for (auto cur = first; cur != last; ++cur, ++i)
        {
            vtmp[i] = (*cur)(index);
        }
        med(index) = internal::direct_median(vtmp);
    }
    return med;
}

// template <typename RAIter>
// typename std::enable_if <
// is_iterable<typename std::iterator_traits<RAIter>::value_type>
// ::value,
// typename std::iterator_traits<RAIter>::value_type >::type
// median(RAIter first, RAIter last)
// {
//     using ValueType = typename std::iterator_traits<RAIter>::value_type;
//     using ScalarType = typename std::remove_reference<decltype(std::declval<ValueType>()[0])>::type;
//     // performs out-of-place median search
//     std::vector<ScalarType> vtmp(std::distance(first, last));
//     ValueType med = *first;
//     unsigned int size = med.size();

//     for (int index = 0; index < size; ++index)
//     {
//         for (int i = 0; first != last; ++first, ++i)
//             vtmp[i] = (*first)[index];
//         med[index] = internal::direct_median(vtmp);
//     }
//     return med;
// }

template <typename T>
T median(const std::vector<T> &v)
{
    // performs out-of-place median search
    std::vector<T> vcpy(v);
    return internal::direct_median(vcpy);
}

template <
    typename T,
    typename = typename std::enable_if<is_iterable<T>::value >::type
    >
T median(const std::vector<T> &v)
{
    using ScalarType = decltype(v[0][0]);
    // performs out-of-place median search
    std::vector<ScalarType> vtmp(v.size());
    T med = v[0];
    unsigned int size = v[0][0].size();

    for (int index = 0; index < size; ++index)
    {
        for (int i = 0; i < v.size(); ++i)
            vtmp[i] = v[i][index];
        med[index] = internal::direct_median(vtmp);
    }
    return med;
}

// template <typename Derived>
// Derived median(const std::vector<DenseExpr<Derived>> &v)
// {
//     using ScalarType = typename DenseExpr<Derived>::Scalar;
//     // performs out-of-place median search
//     std::vector<ScalarType> vtmp(v.size());
//     auto n_rows = v[0].rows();
//     auto n_cols = v[0].cols();
//     Derived med(n_rows, n_cols);

//     for (int row = 0; row < n_rows; ++row)
//         for (int col = 0; col < n_cols; ++col)
//         {
//             for (int i = 0; i < v.size(); ++i)
//                 vtmp[i] = v[i](row, col);
//             med(row, col) = internal::direct_median(vtmp);
//         }
//     return med;
// }

// template <typename Derived>
// Matrix<Derived> median(const DenseExpr<Derived> &v, int dim = 0)
// {
//     assert(dim < 2);
//     using ScalarType = typename Derived::Scalar;
//     // performs out-of-place median search
//     std::vector<ScalarType> vtmp(v.size());
//     Matrix<Derived> med;

//     if (dim == 0) // compute median in each column
//     {
//         med.resize(1, v.cols());
//         for (int col = 0; index < v.cols(); ++col)
//         {
//             for (int i = 0; i < v.size(); ++i)
//                 vtmp[i] = v[i](index);
//             med(row, col) = internal::direct_median(vtmp);
//         }
//     }
//     else
//     {
//         for (int index = 0; index < med.size(); ++index)
//         {
//             for (int i = 0; i < v.size(); ++i)
//                 vtmp[i] = v[i](index);
//             med(row, col) = internal::direct_median(vtmp);
//         }
//     }
//     return med;
// }

template<typename Derived>
typename MatrixExpr<Derived>::Scalar mean(const MatrixExpr<Derived>& mat)
{
 typedef typename MatrixExpr<Derived>::Scalar Scalar;

      return mat.redux(&REDUX::sum<Scalar>) / static_cast<double>(mat.size());
}

template<typename Derived>
typename MatrixExpr<Derived>::Scalar variance(const MatrixExpr<Derived>& mat)
{
 typedef typename MatrixExpr<Derived>::Scalar Scalar;
 Scalar res;
 double len = mat.size();
 auto mat_sum = mat.redux(&REDUX::sum<Scalar>);

 res = ( mat.binaryExpr(mat, &REDUX::prod<Scalar>).redux(&REDUX::sum<Scalar>)
         - REDUX::prod(mat_sum, mat_sum) / (len) )
         / (len - 1);

 return res;
}

END_NAMESPACE // LQCDA

#endif // STATISTICS_HPP