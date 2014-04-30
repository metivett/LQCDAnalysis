/*
 * FitInterface.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: Thibaut Metivet
 */

 #include "FitInterface.hpp"

 using namespace LQCDA;

 FitInterface::FitInterface(unsigned int npts, unsigned int xdim, unsigned int ydim)
 {
 	resize(npts, xdim, ydim);
 }

 void FitInterface::resize(unsigned int npts, unsigned int xdim, unsigned int ydim)
 {
 	_isFitPoint.setConstant(npts, 0);
 	_isXExact.setConstant(xdim, 1);
 	_isXXCorr.setIdentity(xdim, xdim);
 	_isYYCorr.setIdentity(ydim, ydim);
 	_isXYCorr.setZero(xdim, ydim);
 	_isDataCorr.setIdentity(npts, npts);
 	_nPts = npts;
 	_xDim = xdim;
 	_yDim = ydim;
 }

 void FitInterface::fitPoint(const index_t i, const bool b)
 {
 	_isFitPoint(i) = b ? 1 : 0;
 }
 void FitInterface::fitPointRange(const index_t i, const index_t j, const bool b)
 {
 	unsigned int len = static_cast<unsigned int>(j - i + 1);
 	_isFitPoint.segment(i, len).setConstant(b ? 1 : 0);
 }
 void FitInterface::fitAllPoints(const bool b)
 {
 	unsigned int bi = b ? 1 : 0;
 	FOR_VEC(_isFitPoint, i)
 	{
 		_isFitPoint(i) = bi;
 	}
 }
 void FitInterface::assumeXExact(const index_t i, const bool b)
 {
 	_isXExact(i) = b ? 1 : 0;
 }
 void FitInterface::assumeXXCorrelated(const index_t k1, const index_t k2, const bool b)
 {
 	unsigned int val = b ? 1 : 0;
 	_isXXCorr(k1, k2) = val;
 	_isXXCorr(k2, k1) = val;
 }
 void FitInterface::assumeYYCorrelated(const index_t k1, const index_t k2, const bool b)
 {
 	unsigned int val = b ? 1 : 0;
 	_isYYCorr(k1, k2) = val;
 	_isYYCorr(k2, k1) = val;
 }
 void FitInterface::assumeXYCorrelated(const index_t k1, const index_t k2, const bool b)
 {
 	_isXYCorr(k1, k2) = b ? 1 : 0;
 }
 void FitInterface::assumeDataCorrelated(const index_t i1, const index_t i2, const bool b)
 {
 	unsigned int val = b ? 1 : 0;
 	_isDataCorr(i1, i2) = val;
 	_isDataCorr(i2, i1) = val;
 }