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
 	_isFitPoint.setConstant(npts, 1);
 	_isXExact.setConstant(xdim, 1);
 	_isXXCorr.matrix().setIdentity(xdim, xdim);
 	_isYYCorr.matrix().setIdentity(ydim, ydim);
 	_isXYCorr.setZero(xdim, ydim);
 	_isDataCorr.matrix().setIdentity(npts, npts);
 	_nPts = npts;
 	_xDim = xdim;
 	_yDim = ydim;
 }

 void FitInterface::fitPoint(const index_t i, const bool b)
 {
 	_isFitPoint(i) = b ? 1 : 0;
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