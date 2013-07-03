/*
 * FitDataWrapper.hpp
 *
 *  Created on: Jun 25, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_DATA_WRAPPER_HPP_
#define FIT_DATA_WRAPPER_HPP_

#include "FitData.hpp"

namespace LQCDA {

    template<class DataT, class XT>
    class FitDataWrapper : public FitDataA<DataT, XT>
    {
    private:
	typedef FitDataA<DataT, XT> Base;

	std::vector<const DataT*> _Data; // data ("y-points") to be fitted
	std::vector<const XT*> _X; // "x-points" ie points s.t. we evalutate model(x,params)

	std::vector<unsigned int> _EnabledPointsIndexes;
	
	const Eigen::MatrixXd* _CovYY;
	const Eigen::MatrixXd* _CovXX;
	const Eigen::MatrixXd* _CovXY;

    public:
	FitDataWrapper(const std::vector<const DataT*>& data,
		       const std::vector<const XT*>& x,
		       const Eigen::MatrixXd* Cyy,
		       const Eigen::MatrixXd* Cxx,
		       const Eigen::MatrixXd* Cxy) :
	    _Data(data),
	    _X(x),
	    _EnabledPointsIndexes(data.size()),
	    _CovYY(Cyy),
	    _CovXX(Cxx),
	    _CovXY(Cxy)
	    {
		for(int i = 0; i < data.size(); ++i)
		    _EnabledPointsIndexes[i] = i;
	    }
	FitDataWrapper(const DataT** data,
		       const XT** x,
		       unsigned int ndata,
		       const Eigen::MatrixXd* Cyy,
		       const Eigen::MatrixXd* Cxx,
		       const Eigen::MatrixXd* Cxy) :
	    _Data(data, data + ndata),
	    _X(x, x + ndata),
	    _EnabledPointsIndexes(ndata),
	    _CovYY(Cyy),
	    _CovXX(Cxx),
	    _CovXY(Cxy)
	    {
		for(int i = 0; i < ndata; ++i)
		    _EnabledPointsIndexes[i] = i;
	    }

	~FitDataWrapper() {}

	// Getters
	virtual int nData() const { return _EnabledPointsIndexes.size(); }
	virtual int nxDim() const { return SizeInfo(_X[0]); }
	virtual int xSize() const { return nData() * nxDim(); }
	virtual int nyDim() const { return SizeInfo(_Data[0]); }
	virtual int ySize() const { return nData() * nyDim(); }

	virtual const DataT& y(size_t i) const { return *(_Data[_EnabledPointsIndexes[i]]); }
	virtual const XT& x(size_t i) const { return *(_X[_EnabledPointsIndexes[i]]); }

	virtual void DisablePoint(unsigned int i) {
	    assert(i < _Data.size());
	    _EnabledPointsIndexes.erase(std::remove(_EnabledPointsIndexes.begin(), _EnabledPointsIndexes.end(), i), _EnabledPointsIndexes.end());
	}
	virtual void EnablePoint(unsigned int i) {
	    assert(i < _Data.size());
	    _EnabledPointsIndexes.insert(std::find_if(_EnabledPointsIndexes.begin(), _EnabledPointsIndexes.end(), std::bind2nd(std::less<int>(), i)), i);
	}
	virtual void EnableAllPoints() {
	    _EnabledPointsIndexes.resize(_Data.size());
	    for(int i = 0; i < _Data.size(); ++i)
		_EnabledPointsIndexes[i] = i;
	}

    protected:
	unsigned int EnabledPointIndex(unsigned int i) {
	    return _EnabledPointsIndexes[i];
	}
	
    private:
	virtual Eigen::MatrixXd Get_C_yy(unsigned int i, unsigned int j) {
	    return _CovYY->block(EnabledPointIndex(i)*nyDim(), EnabledPointIndex(j)*nyDim(), nyDim(), nyDim());
	}

	virtual Eigen::MatrixXd Get_C_xx(unsigned int i, unsigned int j) {
	    return _CovXX->block(EnabledPointIndex(i)*nxDim(), EnabledPointIndex(j)*nxDim(), nxDim(), nxDim());
	}

	virtual Eigen::MatrixXd Get_C_xy(unsigned int i, unsigned int j) {
	    return _CovXY->block(EnabledPointIndex(i)*nxDim(), EnabledPointIndex(j)*nyDim(), nxDim(), nyDim());
	}
	
// TODO
	virtual bool GetIsCorrelatedXDim(size_t k) const {
	    return true;
	}
	virtual bool GetHaveXCorrelation() const {
	    return !(_CovXX->isZero());
	}
    
    };


} // namespace LQCDA

#endif	// FIT_DATA_WRAPPER_HPP_
