/*
 * fit_fcn.cpp
 *
 *  Created on: Apr 2, 2013
 *      Author: Thibaut Metivet
 */

#include "fit_fcn.hpp"

using namespace LQCDA;

double Chi2Base::operator() (const std::vector<double>& params) const
{
    double res(0);

    size_t nData = _FitData->nData();
    size_t ySize = _FitData->ySize();
    size_t nyDim = _FitData->nyDim();
    size_t xSize = _FitData->xSize();
    size_t nxDim = _FitData->nxDim();
    size_t nModelParams = _FitModel->NbOfParameters();

    // Compute Y and X in case of correlated "x data" and process by blocks
    if(_FitData->HaveXCorrelation()) // CHECK!!!
    {
	std::vector<double> modelParams(params.begin(), params.begin() + nModelParams);
	std::vector<double> dummyParams(params.begin() + nModelParams, params.end());
	
	Eigen::VectorXd X(xSize);
	Eigen::VectorXd Y(ySize);
	std::vector<std::vector<double> > x_buf(nData);
	size_t px_ind (0);	// WRONG!!!
	for(int k=0; k<nxDim; ++k)
	{
	    if(_FitData->IsCorrelatedXDim(k)) {
		for(int i=0; i<nData; ++i)
		{
		    x_buf[i].push_back(dummyParams[px_ind]);
		    X(k+i*nxDim) = (dummyParams[px_ind] - _FitData->x(i,k));
		    px_ind++;
		}
	    }
	    else {
		for(int i=0; i<nData; ++i) {
		    x_buf[i].push_back(_FitData->x(i,k));
		    X(k+i*nxDim) = (double)(0);
		}
	    }
	}
	//LQCDOut<<"Dummy Params"<<" = "<<dummyParams<<'\n'<<'\n';
	//LQCDOut<<"X buf"<<" = "<<x_buf<<'\n'<<'\n';
	//LQCDOut<<"Eval "<<24<<" = "<<std::scientific<<_m_model->Eval(x_buf[24], modelParams)<<'\n'<<'\n';
	
	for(int i=0; i<nData; ++i)
	{
	    for(int k=0; k<nyDim; ++k)
	    {
		Y(k+i*nyDim) = (_FitModel->Eval(k, x_buf[i], modelParams) - _FitData->y(i,k));
	    }
	}

	//LQCDOut<<"Model params "<<" = "<<modelParams<<'\n'<<'\n';
	LQCDOut<<"Y "<<" = "<<Y<<'\n'<<'\n';	
	res = X.transpose() * _m_C_inv_xx * X;
	//LQCDOut<<"Chi 2"<<" = "<<res<<" + ";
	res += Y.transpose() * _m_C_inv_yy * Y;
	//LQCDOut<<res<<" + ";
	res += 2.0 * X.transpose() * _m_C_inv_xy * Y;
	//LQCDOut<<res<<'\n'<<'\n';
    }
    // Else compute only Y and process by blocks
    else
    {
	Eigen::VectorXd Y(ySize);
	for(int i=0; i<nData; ++i)    
	{
	    for(int k=0; k<nyDim; ++k)
	    {
		Y(k+i*nyDim) = (_FitModel->Eval(k, _FitData->x(i), params) - _FitData->y(i,k));
	    }
	}
	//LQCDOut<<"Y = "<<'\n'<<Y<<'\n'<<'\n';
	res = Y.transpose() * _m_C_inv_yy * Y;
    }

    last_value = res;
    //LQCDOut<<"Chi2 = "<<res<<'\n'<<'\n';
    return res;
}

double Chi2Base::DOF() const
{
    unsigned int dof, nbparameters;
    unsigned int nData = _FitData->nData();
    nbparameters = _FitModel->NbOfParameters();
    if(_FitData->HaveXCorrelation()) {
	unsigned int nxDim = _FitData->nxDim();
	for(unsigned int k=0; k<nxDim; ++k) {
	    if(_FitData->IsCorrelatedXDim(k)) {
		nbparameters += nData;
	    }
	}
    }
    
    dof = _FitData->nData() * _FitData->nyDim() - nbparameters;
    return dof;
}

void Chi2Base::compute_C_inv(FitDataBase* data)
{
    size_t nData = data->nData();
    size_t ySize = data->ySize();
    size_t nyDim = data->nyDim();
    size_t xSize = data->xSize();
    size_t nxDim = data->nxDim();

    if(data->HaveXCorrelation()) {
    
	Eigen::MatrixXd C_yy(nyDim*nData,nyDim*nData);
	Eigen::MatrixXd C_xx(nxDim*nData,nxDim*nData);
	Eigen::MatrixXd C_xy(nxDim*nData,nyDim*nData);
	C_yy.setZero();
	C_xx.setZero();
	C_xy.setZero();
    
	/*for(int i=0; i<nData; ++i) {
	  for(int j=0; j<nData; ++j) {
	  C_yy.block(i * nyDim, j * nyDim, nyDim, nyDim) = data->C_yy(i,j);
	  C_xx.block(i * nxDim, j * nxDim, nxDim, nxDim) = data->C_xx(i,j);
	  C_xy.block(i * nxDim, j * nyDim, nxDim, nyDim) = data->C_xy(i,j);
	  }
	  }*/
	for(int i=0; i<nData; ++i) {
	    C_yy.block(i * nyDim, i * nyDim, nyDim, nyDim) = data->C_yy(i,i);
	    C_xx.block(i * nxDim, i * nxDim, nxDim, nxDim) = data->C_xx(i,i);
	    C_xy.block(i * nxDim, i * nyDim, nxDim, nyDim) = data->C_xy(i,i);
	}
   
	// Inversability trick
	for(int i = 0; i < nData; ++i) {
	    for(int k = 0; k < nxDim; ++k) {
		if(C_xx(i * nxDim + k, i * nxDim + k) == 0.)
		    C_xx(i * nxDim + k, i * nxDim + k) = 1.;
	    }
	}

	// Bloc inversion:
	// ( A B )-1   ( X      -XBD-1        )
	// ( C D )   = ( -D-1CX D-1+D-1CXBD-1 )
	// with X=(A-BD-1C)-1
	if(C_yy.determinant() == 0) {
	    _m_C_inv_yy = Eigen::MatrixXd::Identity(C_yy.rows(), C_yy.cols())*100;
	    _m_C_inv_xy = Eigen::MatrixXd::Zero(C_xy.rows(), C_xy.cols());
	    _m_C_inv_xx = C_xx.inverse();
	}
	else {
	    if(C_xy.isZero()) {
		_m_C_inv_xx = C_xx.inverse();
		_m_C_inv_xy = Eigen::MatrixXd::Zero(C_xy.rows(), C_xy.cols());
		_m_C_inv_yy = C_yy.inverse()*0.1;
	    }
	    else {
		Eigen::MatrixXd D_inv = C_yy.inverse();
		_m_C_inv_xx = (C_xx - C_xy * D_inv * C_xy.transpose()).inverse();
		_m_C_inv_xy = - _m_C_inv_xx * C_xy * D_inv;
		_m_C_inv_yy = D_inv + D_inv * C_xy.transpose() * _m_C_inv_xx * C_xy * D_inv;
	    }
	}
    }
    else {			// !data->HaveXCorrelation()
	    
	Eigen::MatrixXd C_yy(nyDim*nData,nyDim*nData);
	C_yy.setZero();

	for(int i=0; i<nData; ++i) {
	    C_yy.block(i * nyDim, i * nyDim, nyDim, nyDim) = data->C_yy(i,i);
	}

	if(C_yy.determinant() == 0) {
	    _m_C_inv_yy = Eigen::MatrixXd::Identity(C_yy.rows(), C_yy.cols())*100;
	}
	else {
	    _m_C_inv_yy = C_yy.inverse();
	}
    }

    
    //LQCDOut<<"C xx inv = "<<'\n'<<_m_C_inv_xx<<'\n'<<'\n';
    //LQCDOut<<"C yy inv = "<<'\n'<<_m_C_inv_yy<<'\n'<<'\n';
    //LQCDOut<<"C xy inv = "<<'\n'<<_m_C_inv_xy<<'\n'<<'\n';
    

}

std::ostream& LQCDA::operator<< (std::ostream& os, const Chi2Base& f)
{
    os<<"Chi2 = "<<(f.LastValue())<<'\n';
    os<<"Chi2 / DOF = "<<(f.LastValue() / f.DOF())<<'\n';
    return os;
}
