/*
 * FitInterface.hpp
 *
 *  Created on: Mar 19, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FIT_INTERFACE_HPP
#define FIT_INTERFACE_HPP

 #include "Globals.hpp"

 namespace LQCDA {

 	class FitInterface
 	{
 	private:
 		Vector<unsigned int> _isFitPoint, _isXExact;
 		Array<bool> _isXXCorr, _isYYCorr, _isXYCorr, _isDataCorr;
 		unsigned int _nPts, _xDim, _yDim;

 	public:
 		// Constructors
 		FitInterface() = default;
 		FitInterface(unsigned int npts, unsigned int xdim, unsigned int ydim);
 		// Destructor
 		virtual ~FitInterface() = default;

 		// Accessors
 		void resize(unsigned int npts, unsigned int xdim, unsigned int ydim);

 		void fitPoint(const index_t i, const bool b =true);
 		void assumeXExact(const index_t i, const bool b =true);
 		void assumeXXCorrelated(const index_t k1, const index_t k2, const bool b =true);
 		void assumeYYCorrelated(const index_t k1, const index_t k2, const bool b =true);
 		void assumeXYCorrelated(const index_t k1, const index_t k2, const bool b =true);
 		void assumeDataCorrelated(const index_t i1, const index_t i2, const bool b =true);

 		bool isFitPoint(const index_t i) const { return _isFitPoint(i); }
 		bool isXExact(const index_t k) const { return _isXExact(k); }
 		bool isXXCorrelated(const index_t k1, const index_t k2) const { return _isXXCorr(k1, k2); }
 		bool isYYCorrelated(const index_t k1, const index_t k2) const { return _isYYCorr(k1, k2); }
 		bool isXYCorrelated(const index_t k1, const index_t k2) const { return _isXYCorr(k1, k2); }
 		bool isDataCorrelated(const index_t i1, const index_t i2) const { return _isDataCorr(i1, i2); }

 		// unsigned int nPoints() const { return _nPts; }
 		unsigned int nFitPoints() const { return _isFitPoint.sum(); }
 		// unsigned int xDim() const { return _xDim; }
 		unsigned int nFitXDim() const { return _xDim - _isXExact.sum(); }
 		// unsigned int yDim() const { return _yDim; }

 	};

 }

#endif // FIT_INTERFACE_HPP