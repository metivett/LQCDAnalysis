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

    size_t nData = _m_fitdata->nData();
    size_t ySize = _m_fitdata->ySize();
    size_t nyDim = _m_fitdata->nyDim();
    size_t xSize = _m_fitdata->xSize();
    size_t nxDim = _m_fitdata->nxDim();
    size_t nModelParams = _m_model->NbOfParameters();

    // Compute Y and X in case of correlated "x data" and process by blocks
    if(_m_fitdata->HaveXCorrelation()) // CHECK!!!
    {
	std::vector<double> modelParams(params.begin(), params.begin() + nModelParams);
	std::vector<double> dummyParams(params.begin() + nModelParams, params.end());
	
	Eigen::VectorXd X(xSize);
	Eigen::VectorXd Y(ySize);
	std::vector<std::vector<double> > x_buf(nData);
	size_t px_ind (0);	// WRONG!!!
	for(int k=0; k<nxDim; ++k)
	{
	    if(_m_fitdata->IsCorrelatedXDim(k)) {
		for(int i=0; i<nData; ++i)
		{
		    x_buf[i].push_back(dummyParams[px_ind]);
		    X(k+i*nxDim) = (dummyParams[px_ind] - _m_fitdata->x(i,k));
		    px_ind++;
		}
	    }
	    else {
		for(int i=0; i<nData; ++i) {
		    x_buf[i].push_back(_m_fitdata->x(i,k));
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
		Y(k+i*nyDim) = (_m_model->Eval(k, x_buf[i], modelParams) - _m_fitdata->y(i,k));
	    }
	}

	//LQCDOut<<"Model params "<<" = "<<modelParams<<'\n'<<'\n';
	//LQCDOut<<"Y "<<" = "<<Y<<'\n'<<'\n';	
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
		Y(k+i*nyDim) = (_m_model->Eval(k, _m_fitdata->x(i), params) - _m_fitdata->y(i,k));
	    }
	}
	res = Y.transpose() * _m_C_inv_yy * Y;
    }

    last_value = res;
    return res;
}

size_t Chi2Base::getDOF() const
{
    size_t dof;
    dof = _m_fitdata->nData() * _m_fitdata->nyDim() - _m_model->NbOfParameters();
    return dof;
}

void Chi2Base::compute_C_inv(FitDataBase* data)
{
    Eigen::MatrixXd C_yy = data->C_yy();
    Eigen::MatrixXd C_xx = data->C_xx();
    Eigen::MatrixXd C_xy = data->C_xy();

    size_t nData = data->nData();
    size_t ySize = data->ySize();
    size_t nyDim = data->nyDim();
    size_t xSize = data->xSize();
    size_t nxDim = data->nxDim();
   
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
	_m_C_inv_yy = Eigen::MatrixXd::Identity(C_yy.rows(), C_yy.cols())*10000;
	_m_C_inv_xy = Eigen::MatrixXd::Zero(C_xy.rows(), C_xy.cols());
	_m_C_inv_xx = C_xx.inverse();
    }
    else {
	if(C_xy.isZero()) {
	    _m_C_inv_xx = C_xx.inverse();
	    _m_C_inv_xy = Eigen::MatrixXd::Zero(C_xy.rows(), C_xy.cols());
	    _m_C_inv_yy = C_yy.inverse();
	}
	else {
	    Eigen::MatrixXd D_inv = C_yy.inverse();
	    _m_C_inv_xx = (C_xx - C_xy * D_inv * C_xy.transpose()).inverse();
	    _m_C_inv_xy = - _m_C_inv_xx * C_xy * D_inv;
	    _m_C_inv_yy = D_inv + D_inv * C_xy.transpose() * _m_C_inv_xx * C_xy * D_inv;
	}
    }

    /*
    LQCDOut<<"C xx inv = "<<'\n'<<_m_C_inv_xx<<'\n'<<'\n';
    LQCDOut<<"C yy inv = "<<'\n'<<_m_C_inv_yy<<'\n'<<'\n';
    LQCDOut<<"C xy inv = "<<'\n'<<_m_C_inv_xy<<'\n'<<'\n';
    */

}

std::ostream& LQCDA::operator<< (std::ostream& os, const Chi2Base& f)
{
    os<<"Chi2/DOF = "<<(f.getLastValue() / f.getDOF());
    return os;
}
