/*
 * XYDataSample.hpp
 *
 *  Created on: Apr 10, 2014
 *      Author: Thibaut Metivet
 */

#ifndef XY_DATA_SAMPLE_HPP
#define XY_DATA_SAMPLE_HPP

#include "Sample.hpp"
#include "XYDataMap.hpp"
#include "XYData.hpp"

namespace LQCDA
{

template<typename T>
class XYDataSample
{
protected:
    // Typedefs
    typedef typename XYDataInterface<T>::range range;
    typedef Ref<Matrix<T>> block_t;
    typedef ConstRef<Matrix<T>> const_block_t;
    typedef Sample<Matrix<T>> MatrixSample;
    typedef typename MatrixSample::BlockSample BlockSample;
    typedef typename MatrixSample::ConstBlockSample ConstBlockSample;
    typedef typename MatrixSample::ScalarSample ScalarSample;
    typedef typename MatrixSample::ConstScalarSample ConstScalarSample;

    // Data
    unsigned int _nPts, _xDim, _yDim;
    MatrixSample _x, _y;
    mutable Matrix<Matrix<T>> _Cxx, _Cyy, _Cxy;
    mutable bool is_Cxx_updated, is_Cyy_updated, is_Cxy_updated;

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

    unsigned int nPoints() const
    {
        return _nPts;
    }
    unsigned int xDim() const
    {
        return _xDim;
    }
    unsigned int yDim() const
    {
        return _yDim;
    }

    ScalarSample x(index_t i, index_t k);
    ConstScalarSample x(index_t i, index_t k) const;
    BlockSample x(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2);
    ConstBlockSample x(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) const;
    BlockSample x(index_t i, std::initializer_list<index_t> r2);
    ConstBlockSample x(index_t i, std::initializer_list<index_t> r2) const;
    BlockSample x(std::initializer_list<index_t> r1, index_t k);
    ConstBlockSample x(std::initializer_list<index_t> r1, index_t k) const;

    ScalarSample y(index_t i, index_t k);
    ConstScalarSample y(index_t i, index_t k) const;
    BlockSample y(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2);
    ConstBlockSample y(std::initializer_list<index_t> r1, std::initializer_list<index_t> r2) const;
    BlockSample y(index_t i, std::initializer_list<index_t> r2);
    ConstBlockSample y(index_t i, std::initializer_list<index_t> r2) const;
    BlockSample y(std::initializer_list<index_t> r1, index_t k);
    ConstBlockSample y(std::initializer_list<index_t> r1, index_t k) const;

    block_t xxCov(index_t k1, index_t k2);
    const_block_t xxCov(index_t k1, index_t k2) const;
    block_t yyCov(index_t k1, index_t k2);
    const_block_t yyCov(index_t k1, index_t k2) const;
    block_t xyCov(index_t k1, index_t k2);
    const_block_t xyCov(index_t k1, index_t k2) const;

    void setCovFromSample() const;

    const XYDataMap<T> getData(unsigned int s);

    // Operators
    XYDataMap<T> operator[](unsigned int s);
    const XYDataMap<T> operator[](unsigned int s) const;

    // Statistics
    XYData<T> mean(unsigned int begin = 0, int n = -1) const;

private:
    void update_Cxx() const;
    void update_Cyy() const;
    void update_Cxy() const;

    void init_Cxx() const;
    void init_Cyy() const;
    void init_Cxy() const;

    range check_range(std::initializer_list<index_t> r, unsigned int max) const;
};

template<typename T>
XYDataSample<T>::XYDataSample()
    : is_Cxx_updated {false}
, is_Cyy_updated {false}
, is_Cxy_updated {false}
{}

template<typename T>
XYDataSample<T>::XYDataSample(
    unsigned int npts, unsigned int xdim,
    unsigned int ydim, unsigned int nsamples)
    : is_Cxx_updated {false}
, is_Cyy_updated {false}
, is_Cxy_updated {false}
{
    this->resize(npts, xdim , ydim, nsamples);
}

template<typename T>
unsigned int XYDataSample<T>::size() const
{
    return _x.size();
}

template<typename T>
void XYDataSample<T>::resize(
    unsigned int npts, unsigned int xdim,
    unsigned int ydim, unsigned int nsamples)
{
    _nPts = npts;
    _xDim = xdim;
    _yDim = ydim;
    _x.resize(nsamples);
    _x.resizeMatrix(npts, xdim);
    _y.resize(nsamples);
    _y.resizeMatrix(npts, ydim);
    init_Cxx();
    init_Cyy();
    init_Cxy();
    is_Cxx_updated = false;
    is_Cyy_updated = false;
    is_Cxy_updated = false;
}

template<typename T>
typename XYDataSample<T>::ScalarSample XYDataSample<T>::x(index_t i, index_t k)
{
    is_Cxx_updated = false;
    is_Cxy_updated = false;
    return _x(i, k);
}

template<typename T>
typename XYDataSample<T>::ConstScalarSample XYDataSample<T>::x(index_t i, index_t k) const
{
    return _x(i, k);
}

template<typename T>
typename XYDataSample<T>::BlockSample XYDataSample<T>::x(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2)
{
    auto r1 = check_range(l1, _nPts);
    auto r2 = check_range(l2, _xDim);
    is_Cxx_updated = false;
    is_Cxy_updated = false;
    return _x.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
}

template<typename T>
typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::x(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2) const
{
    auto r1 = check_range(l1, _nPts);
    auto r2 = check_range(l2, _xDim);
    return _x.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
}

template<typename T>
typename XYDataSample<T>::BlockSample XYDataSample<T>::x(index_t i, std::initializer_list<index_t> l2)
{
    auto r2 = check_range(l2, _xDim);
    is_Cxx_updated = false;
    is_Cxy_updated = false;
    return _x.block(i, r2[0], 1, r2[1] - r2[0]);
}

template<typename T>
typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::x(index_t i, std::initializer_list<index_t> l2) const
{
    auto r2 = check_range(l2, _xDim);
    return _x.block(i, r2[0], 1, r2[1] - r2[0]);
}

template<typename T>
typename XYDataSample<T>::BlockSample XYDataSample<T>::x(std::initializer_list<index_t> l1, index_t k)
{
    auto r1 = check_range(l1, _nPts);
    is_Cxx_updated = false;
    is_Cxy_updated = false;
    return _x.block(r1[0], k, r1[1] - r1[0], 1);
}

template<typename T>
typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::x(std::initializer_list<index_t> l1, index_t k) const
{
    auto r1 = check_range(l1, _nPts);
    return _x.block(r1[0], k, r1[1] - r1[0], 1);
}

template<typename T>
typename XYDataSample<T>::ScalarSample XYDataSample<T>::y(index_t i, index_t k)
{
    is_Cyy_updated = false;
    is_Cxy_updated = false;
    return _y(i, k);
}

template<typename T>
typename XYDataSample<T>::ConstScalarSample XYDataSample<T>::y(index_t i, index_t k) const
{
    return _y(i, k);
}

template<typename T>
typename XYDataSample<T>::BlockSample XYDataSample<T>::y(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2)
{
    auto r1 = check_range(l1, _nPts);
    auto r2 = check_range(l2, _yDim);
    is_Cyy_updated = false;
    is_Cxy_updated = false;
    return _y.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
}

template<typename T>
typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::y(std::initializer_list<index_t> l1, std::initializer_list<index_t> l2) const
{
    auto r1 = check_range(l1, _nPts);
    auto r2 = check_range(l2, _yDim);
    return _y.block(r1[0], r2[0], r1[1] - r1[0], r2[1] - r2[0]);
}

template<typename T>
typename XYDataSample<T>::BlockSample XYDataSample<T>::y(index_t i, std::initializer_list<index_t> l2)
{
    auto r2 = check_range(l2, _yDim);
    is_Cyy_updated = false;
    is_Cxy_updated = false;
    return _y.block(i, r2[0], 1, r2[1] - r2[0]);
}

template<typename T>
typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::y(index_t i, std::initializer_list<index_t> l2) const
{
    auto r2 = check_range(l2, _yDim);
    return _y.block(i, r2[0], 1, r2[1] - r2[0]);
}

template<typename T>
typename XYDataSample<T>::BlockSample XYDataSample<T>::y(std::initializer_list<index_t> l1, index_t k)
{
    auto r1 = check_range(l1, _nPts);
    is_Cyy_updated = false;
    is_Cxy_updated = false;
    return _y.block(r1[0], k, r1[1] - r1[0], 1);
}

template<typename T>
typename XYDataSample<T>::ConstBlockSample XYDataSample<T>::y(std::initializer_list<index_t> l1, index_t k) const
{
    auto r1 = check_range(l1, _nPts);
    return _y.block(r1[0], k, r1[1] - r1[0], 1);
}

template<typename T>
void XYDataSample<T>::setCovFromSample() const
{
    update_Cxx();
    update_Cyy();
    update_Cxy();
}

template<typename T>
const XYDataMap<T> XYDataSample<T>::getData(unsigned int s)
{
    XYDataMap<T> res(_x[s].data(), _y[s].data(), _nPts, _xDim, _yDim);
    // update_Cxx();
    res.setXXCov(&_Cxx);
    // update_Cyy();
    res.setYYCov(&_Cyy);
    // update_Cxy();
    res.setXYCov(&_Cxy);

    return res;
}

template<typename T>
XYDataMap<T> XYDataSample<T>::operator[](unsigned int s)
{
    XYDataMap<T> res(_x[s].data(), _y[s].data(), _nPts, _xDim, _yDim);
    // update_Cxx();
    res.setXXCov(&_Cxx);
    // update_Cyy();
    res.setYYCov(&_Cyy);
    // update_Cxy();
    res.setXYCov(&_Cxy);

    is_Cxx_updated = false;
    is_Cyy_updated = false;
    is_Cxy_updated = false;

    return res;
}

template<typename T>
const XYDataMap<T> XYDataSample<T>::operator[](unsigned int s) const
{
    XYDataMap<T> res(_x[s].data(), _y[s].data(), _nPts, _xDim, _yDim);
    // update_Cxx();
    res.setXXCov(&_Cxx);
    // update_Cyy();
    res.setYYCov(&_Cyy);
    // update_Cxy();
    res.setXYCov(&_Cxy);

    return res;
}

template<typename T>
typename XYDataSample<T>::block_t XYDataSample<T>::xxCov(index_t k1, index_t k2)
{
    // if(!is_Cxx_updated)
    //  update_Cxx();
    return _Cxx(k1, k2);
}
template<typename T>
typename XYDataSample<T>::const_block_t XYDataSample<T>::xxCov(index_t k1, index_t k2) const
{
    // if(!is_Cxx_updated)
    //  update_Cxx();
    return _Cxx(k1, k2);
}

template<typename T>
typename XYDataSample<T>::block_t XYDataSample<T>::yyCov(index_t k1, index_t k2)
{
    // if(!is_Cyy_updated)
    //  update_Cyy();
    return _Cyy(k1, k2);
}
template<typename T>
typename XYDataSample<T>::const_block_t XYDataSample<T>::yyCov(index_t k1, index_t k2) const
{
    // if(!is_Cyy_updated)
    //  update_Cyy();
    return _Cyy(k1, k2);
}

template<typename T>
typename XYDataSample<T>::block_t XYDataSample<T>::xyCov(index_t k1, index_t k2)
{
    // if(!is_Cxy_updated)
    //  update_Cxy();
    return _Cxy(k1, k2);
}
template<typename T>
typename XYDataSample<T>::const_block_t XYDataSample<T>::xyCov(index_t k1, index_t k2) const
{
    // if(!is_Cxy_updated)
    //  update_Cxy();
    return _Cxy(k1, k2);
}

template<typename T>
void XYDataSample<T>::update_Cxx() const
{
    if (!is_Cxx_updated)
    {
        T n = static_cast<T>(size());
        Sample<Matrix<T>> buf1, buf2;
        FOR_MAT(_Cxx, k1, k2)
        {
            buf1 = _x.col(k1);
            buf2 = _x.col(k2);
            _Cxx(k1, k2) = buf1.covarianceMatrix(buf2);
        }
        is_Cxx_updated = true;
    }
}

template<typename T>
void XYDataSample<T>::update_Cyy() const
{
    if (!is_Cyy_updated)
    {
        // std::cout << "update CYY" << '\n';
        T n = static_cast<T>(size());
        Sample<Matrix<T>> buf1, buf2;
        FOR_MAT(_Cyy, k1, k2)
        {
            buf1 = _y.col(k1);
            buf2 = _y.col(k2);
            _Cyy(k1, k2) = buf1.covarianceMatrix(buf2);
        }
        is_Cyy_updated = true;
    }
}

template<typename T>
void XYDataSample<T>::update_Cxy() const
{
    if (!is_Cxy_updated)
    {
        T n = static_cast<T>(size());
        Sample<Matrix<T>> buf1, buf2;
        FOR_MAT(_Cxy, k1, k2)
        {
            buf1 = _x.col(k1);
            buf2 = _y.col(k2);
            _Cxy(k1, k2) = buf1.covarianceMatrix(buf2);
        }
        is_Cxy_updated = true;
    }
}

template<typename T>
void XYDataSample<T>::init_Cxx() const
{
    _Cxx.resize(_xDim, _xDim);
    FOR_MAT(_Cxx, k1, k2)
    {
        if (k1 == k2)
            _Cxx(k1, k2).setIdentity(_nPts, _nPts);
        else
            _Cxx(k1, k2).setZero(_nPts, _nPts);
    }
}

template<typename T>
void XYDataSample<T>::init_Cyy() const
{
    _Cyy.resize(_yDim, _yDim);
    FOR_MAT(_Cyy, k1, k2)
    {
        if (k1 == k2)
            _Cyy(k1, k2).setIdentity(_nPts, _nPts);
        else
            _Cyy(k1, k2).setZero(_nPts, _nPts);
    }
}

template<typename T>
void XYDataSample<T>::init_Cxy() const
{
    _Cxy.resize(_xDim, _yDim);
    FOR_MAT(_Cxy, k1, k2)
    {
        _Cxy(k1, k2).setZero(_nPts, _nPts);
    }
}

template<typename T>
typename XYDataSample<T>::range XYDataSample<T>::check_range(std::initializer_list<index_t> r, unsigned int max) const
{
    unsigned int range_size = r.size();
    ASSERT(range_size <= 2);
    range res;
    switch (range_size)
    {
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

template<typename T>
XYData<T> XYDataSample<T>::mean(unsigned int begin, int n) const
{
    XYData<T> result(_nPts, _xDim, _yDim);
    result.x({}, {}) = _x.mean(begin, n);
    result.y({}, {}) = _y.mean(begin, n);
    for(int kx1 = 0; kx1 < _xDim; kx1++)
    {
    	for(int kx2 = 0; kx2 < _xDim; kx2++)
    		result.xxCov(kx1, kx2) = xxCov(kx1, kx2);
    	for(int ky1 = 0; ky1 < _yDim; ky1++)
    		result.xyCov(kx1, ky1) = xyCov(kx1, ky1);
    }
    for(int ky1 = 0; ky1 < _yDim; ky1++)
    	for(int ky2 = 0; ky2 < _yDim; ky2++)
    		result.yyCov(ky1, ky2) = yyCov(ky1, ky2);
    
    return result;
}

//     template<typename T>
//     T XYDataSample::covariance(const XYDataSample& sample, unsigned int begin, int n) const
//     {
//         const unsigned int len = (n >= 0)? n: size();
//         if(len)
//         {
//             auto sampleV = sample.segment(begin, len);
//             auto thisV = this->segment(begin, len);

//             // COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
//             return ( thisV.binaryExpr(sampleV, &REDUX::prod<T>).redux(&REDUX::sum<T>)
//                 - REDUX::prod(thisV.redux(&REDUX::sum<T>), sampleV.redux(&REDUX::sum<T>)) / static_cast<double>(len) )
//                 / static_cast<double>(len - 1);
//         }
//     }

//     template<typename T>
//     auto XYDataSample::covarianceMatrix(const XYDataSample& sample, unsigned int begin, int n) const
//     -> decltype(REDUX::tensorProd(std::declval<XYDataSample>().segment(begin, n).redux(&REDUX::sum<T>), sample.segment(begin, n).redux(&REDUX::sum<T>)))
//     {
//         const unsigned int len = (n >= 0)? n: size();
//         if(len)
//         {
//             auto sampleV = sample.segment(begin, len);
//             auto thisV = this->segment(begin, len);

//             // COV(X, Y) = 1/(n-1) (sum(xi * yi) - 1/n (sum(xi) sum(yi)))
//             return ( thisV.binaryExpr(sampleV, &REDUX::tensorProd<T>).redux(&REDUX::sum<T>)
//                 - REDUX::tensorProd(thisV.redux(&REDUX::sum<T>), sampleV.redux(&REDUX::sum<T>)) / static_cast<double>(len) )
//                 / static_cast<double>(len - 1);
//         }
//     }

//     template<typename T>
//     T XYDataSample::variance(unsigned int begin, int n) const
//     {
//         return covariance(*this, begin, n);
//     }

//     template<typename T>
//     auto XYDataSample::varianceMatrix(unsigned int begin, int n) const
//     -> decltype(std::declval<XYDataSample>().covarianceMatrix(std::declval<XYDataSample>(), begin, n))
// {
//  return covarianceMatrix(*this, begin, n);
// }

}

#endif // XY_DATA_SAMPLE_HPP