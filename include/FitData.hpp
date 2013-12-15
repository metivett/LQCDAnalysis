/*
 * FitData.hpp
 *
 *  Created on: Apr 15, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_DATA_HPP_
#define FIT_DATA_HPP_

#include <vector>
#include "Eigen/Dense"
#include "GetValue.hpp"
#include "SizeInfo.hpp"
#include "io_utils.hpp"

namespace LQCDA {

    template<class DataT, class XT>
    class FitDataA
    {
    public:
	typedef DataT data_type;
	typedef XT x_type;
	
	// Getters
	virtual int nData() const =0;
	virtual int nxDim() const =0;
	virtual int xSize() const =0;
	virtual int nyDim() const =0;
	virtual int ySize() const =0;

	virtual const DataT& y(size_t i) const =0;
	virtual const XT& x(size_t i) const =0;

	virtual void DisablePoint(unsigned int i) =0;
	virtual void EnablePoint(unsigned int i) =0;
	virtual void EnableAllPoints() =0;

	Eigen::MatrixXd C_yy(unsigned int i, unsigned int j) { return Get_C_yy(i,j); }
	Eigen::MatrixXd C_xx(unsigned int i, unsigned int j) { return Get_C_xx(i,j); }
	Eigen::MatrixXd C_xy(unsigned int i, unsigned int j) { return Get_C_xy(i,j); }

	bool IsCorrelatedXDim(size_t k) const { return GetIsCorrelatedXDim(k); }
	bool HaveXCorrelation() const { return GetHaveXCorrelation(); }

    private:
	virtual Eigen::MatrixXd Get_C_yy(unsigned int i, unsigned int j) =0;
	virtual Eigen::MatrixXd Get_C_xx(unsigned int i, unsigned int j) =0;
	virtual Eigen::MatrixXd Get_C_xy(unsigned int i, unsigned int j) =0;
	virtual bool GetIsCorrelatedXDim(size_t k) const =0;
	virtual bool GetHaveXCorrelation() const =0;
    };

    template<class DataT, class XT>
    class FitDataBase : public FitDataA<DataT, XT>
    {
    private:
	std::vector<DataT> _Data; // data ("y-points") to be fitted
	std::vector<XT> _X; // "x-points" ie points s.t. we evaluate model(x,params)

	std::vector<unsigned int> _EnabledPointsIndexes;

    public:
	typedef DataT data_type;
	typedef XT x_type;
	
    public:
	FitDataBase() {}
	FitDataBase(const std::vector<DataT>& data,
		    const std::vector<XT>& x) :
	    _Data(data), _X(x),
	    _EnabledPointsIndexes(data.size())
	    {
		for(int i = 0; i < data.size(); ++i)
		    _EnabledPointsIndexes[i] = i;
	    }

	// Getters
	virtual int nData() const { return _EnabledPointsIndexes.size(); }
	virtual int nxDim() const { return SizeInfo(_X[0]); }
	virtual int xSize() const { return nData() * nxDim(); }
	virtual int nyDim() const { return SizeInfo(_Data[0]); }
	virtual int ySize() const { return nData() * nyDim(); }

	/*std::vector<double> y(size_t i) const { return static_cast<std::vector<double>>(_Data[_EnabledPointsIndexes[i]]); }
	double y(size_t i, size_t k) const { return static_cast<double>(_Data[_EnabledPointsIndexes[i]], k); }
	std::vector<double> x(size_t i) const { return static_cast<std::vector<double>>(_X[_EnabledPointsIndexes[i]]); }
	double x(size_t i, size_t k) const { return static_cast<double>(_X[_EnabledPointsIndexes[i]], k); }*/
	virtual const DataT& y(size_t i) const { return _Data[_EnabledPointsIndexes[i]]; }
	virtual const XT& x(size_t i) const { return _X[_EnabledPointsIndexes[i]]; }

	virtual void Add(const DataT& data, const XT& x) {
	    _Data.push_back(data);
	    _X.push_back(x);
	    _EnabledPointsIndexes.push_back(_Data.size() - 1);
	}

	void Reserve(size_t n) {
	    _Data.reserve(n);
	    _X.reserve(n);
	    _EnabledPointsIndexes.reserve(n);
	}

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
	const std::vector<DataT>& Data() { return _Data; }
	const std::vector<XT>& X() { return _X; }

	std::vector<DataT> EnabledData() {
	    std::vector<DataT> res(nData());
	    for(unsigned int i = 0; i < nData(); ++i)
		res[i] = _Data[_EnabledPointsIndexes[i]];
	    return res;
	}
	std::vector<XT> EnabledX() {
	    std::vector<XT> res(nData());
	    for(unsigned int i = 0; i < nData(); ++i)
		res[i] = _X[_EnabledPointsIndexes[i]];
	    return res;
	}

    private:
	virtual Eigen::MatrixXd Get_C_yy(unsigned int i, unsigned int j) =0;
	virtual Eigen::MatrixXd Get_C_xx(unsigned int i, unsigned int j) =0;
	virtual Eigen::MatrixXd Get_C_xy(unsigned int i, unsigned int j) =0;
	virtual bool GetIsCorrelatedXDim(size_t k) const =0;
	virtual bool GetHaveXCorrelation() const =0;
    };

    
    template<class DataT, class XT,
	     template<class, class> class YYCovariancePolicy,
	     template<class, class> class XXCovariancePolicy,
	     template<class, class> class XYCovariancePolicy>
    class FitData : public FitDataBase<DataT, XT>
    {
    private:
	typedef FitDataBase<DataT, XT> Base;
	typedef XXCovariancePolicy<XT, XT> XXCov;
	typedef YYCovariancePolicy<DataT, DataT> YYCov;
	typedef XYCovariancePolicy<XT, DataT> XYCov;

	Eigen::MatrixXd _CovYY;
	Eigen::MatrixXd _CovXX;
	Eigen::MatrixXd _CovXY;
	bool _UpToDateCov;

    public:
	FitData() :
	    _UpToDateCov(true)
	    {}
	FitData(const std::vector<DataT>& data,
		const std::vector<XT>& x) :
	    Base(data, x),
	    _CovYY(YYCov::Covariance(data, data)),
	    _CovXX(XXCov::Covariance(x, x)),
	    _CovXY(XYCov::Covariance(x, data)),
	    _UpToDateCov(true)
	    {}

	using Base::nData;
	using Base::nxDim;
	using Base::xSize;
	using Base::nyDim;
	using Base::ySize;

	virtual void Add(const DataT& data, const XT& x) {
	    Base::Add(data, x);
	    _UpToDateCov = false;
	}

    protected:
	using Base::EnabledPointIndex;
	using Base::Data;
	using Base::X;

	const Eigen::MatrixXd& C_yy() {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovYY;
	}

	const Eigen::MatrixXd& C_xx() {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovXX;
	}

	const Eigen::MatrixXd& C_xy() {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovXY;
	}

    private:
	void UpdateCovariance() {
	    _CovYY = YYCov::Covariance(Data(), Data());
	    _CovXX = XXCov::Covariance(X(), X());
	    _CovXY = XYCov::Covariance(X(), Data());
	    _UpToDateCov = true;
	}

	virtual Eigen::MatrixXd Get_C_yy(unsigned int i, unsigned int j) {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovYY.block(EnabledPointIndex(i)*nyDim(), EnabledPointIndex(j)*nyDim(), nyDim(), nyDim());
	}

	virtual Eigen::MatrixXd Get_C_xx(unsigned int i, unsigned int j) {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovXX.block(EnabledPointIndex(i)*nxDim(), EnabledPointIndex(j)*nxDim(), nxDim(), nxDim());
	}

	virtual Eigen::MatrixXd Get_C_xy(unsigned int i, unsigned int j) {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovXY.block(EnabledPointIndex(i)*nxDim(), EnabledPointIndex(j)*nyDim(), nxDim(), nyDim());
	}
	
	// TODO
	virtual bool GetIsCorrelatedXDim(size_t k) const {
	    return true;
	}
	virtual bool GetHaveXCorrelation() const {
	    return !(_CovXX.isZero());
	}
	
    };
    

} // namespace LQCDA


#endif	// FIT_DATA_HPP_
