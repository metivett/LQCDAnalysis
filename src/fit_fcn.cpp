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
	
    // Compute Y and X in case of correlated "x data" and process by blocks
    if(_m_fitdata->have_x_corr()) // CHECK!!!
    {
	Eigen::VectorXd X(xSize);
	Eigen::VectorXd Y(ySize);
	std::vector<std::vector<double> > x_buf(nData);
	size_t px_ind (0);
	for(int k=0; k<nxDim; ++k)
	{
	    if(_m_fitdata->is_x_corr(k)) {
		for(int i=0; i<nData; ++i)
		{
		    x_buf[i].push_back(params[px_ind]);
		    X(k+i*nxDim) = (params[px_ind] - _m_fitdata->x(i,k));
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
	for(int i=0; i<nData; ++i)
	{
	    for(int k=0; k<nyDim; ++k)
	    {
		Y(k+i*nyDim) = (_m_model->Eval(k, x_buf[i], params) - _m_fitdata->y(i,k));
	    }
	}
	res = X.transpose() * _m_C_inv_xx * X;
	res += Y.transpose() * _m_C_inv_yy * Y;
	res += 2.0 * X.transpose() * _m_C_inv_xy * Y;
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
    const Eigen::MatrixXd& C_yy = data->C_yy();
    const Eigen::MatrixXd& C_xx = data->C_xx();
    const Eigen::MatrixXd& C_xy = data->C_xy();

    // Bloc inversion:
    // ( A B )-1   ( X      -XBD-1        )
    // ( C D )   = ( -D-1CX D-1+D-1CXBD-1 )
    // with X=(A-BD-1C)-1
    if(C_yy.determinant() == 0) {
	_m_C_inv_yy = Eigen::MatrixXd::Identity(C_yy.rows(), C_yy.cols());
	_m_C_inv_xy = Eigen::MatrixXd::Zero(C_xy.rows(), C_xy.cols());
	if(C_xx.determinant() == 0) {
	    _m_C_inv_xx = Eigen::MatrixXd::Identity(C_xx.rows(), C_xx.cols());
	}
	else {
	    _m_C_inv_xx = C_xx.inverse();
	}
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

    /*Eigen::MatrixXd C(C_xx.rows()+C_yy.rows(),C_xx.cols()+C_yy.cols());
    C.block(0,0,C_xx.rows(),C_xx.cols()) = C_xx;
    C.block(0,C_xx.cols(),C_xx.rows(),C_yy.cols()) = C_xy;
    C.block(C_xx.rows(),0,C_yy.rows(),C_xx.cols()) = C_xy.transpose();
    C.block(C_xx.rows(),C_xx.cols(),C_yy.rows(),C_yy.cols()) = C_yy;

    Eigen::MatrixXd C_inv = C.inverse();
    LQCDOut<<C_xy;
    _m_C_inv_xx = C_inv.block(0,0,C_xx.rows(),C_xx.cols());
    _m_C_inv_xy = C_inv.block(0,C_xx.cols(),C_xx.rows(),C_yy.cols());
    _m_C_inv_yy = C_inv.block(C_xx.rows(),C_xx.cols(),C_yy.rows(),C_yy.cols());*/
}

std::ostream& LQCDA::operator<< (std::ostream& os, const Chi2Base& f)
{
    os<<"Chi2/DOF = "<<(f.getLastValue() / f.getDOF());
    return os;
}
