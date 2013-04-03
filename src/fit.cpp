/*
 * fit.cpp
 *
 *  Created on: Apr 2, 2013
 *      Author: Thibaut Metivet
 */

#include "fit.hpp"


double LQCDA::Chi2Base::operator() (const std::vector<double>& params)
{
    double res(0);

    size_t nData = _m_fitdata.nData();
    size_t ySize = _m_fitdata.ySize();
    size_t nyDim = _m_fitdata.nyDim();
    size_t xSize = _m_fitdata.xSize();
    size_t nxDim = _m_fitdata.nxDim();
	
    // Compute Y and X in case of correlated "x data" and process by blocks
    if(_m_fitdata.have_x_corr())
    {
	Eigen::VectorXd X(xSize);
	Eigen::VectorXd Y(ySize);
	std::vector<std::vector<double> > x_buf(nData);
	size_t px_ind (0);
	for(int k=0; k<nxDim; ++k)
	{
	    for(int i=0; i<nData; ++i)
	    {
		if(_m_fitdata.is_x_corr(k)) {
		    x_buf[i].push_back(params.at(px_ind++));
		    X<<(params[px_ind] - _m_fitdata._m_x[i][k]);
		}
		else {
		    x_buf[i].push_back(_m_fitdata._m_x[i][k]);
		    X<<(double)0;
		}
	    }
	}
	for(int k=0; k<nyDim; ++k)
	{
	    for(int i=0; i<nData; ++i)
	    {
		Y<<(_m_model.eval(k, x_buf[i], params) - _m_fitdata._m_data[i][k]);
	    }
	}
	res = X.transpose() * _m_C_inv_xx * X
	    + Y.transpose() * _m_C_inv_yy * Y
	    + 2.0 * X.transpose() * _m_C_inv_xy * Y;
    }
    // Else compute only Y and process by blocks
    else
    {
	Eigen::VectorXd Y(ySize);
	for(int k=0; k<nyDim; ++k)	    
	{
	    for(int i=0; i<nData; ++i)
	    {
		Y<<(_m_model.eval(k, _m_fitdata._m_x[i], params) - _m_fitdata._m_data[i][k]);
	    }
	}
	res = Y.transpose() * _m_C_inv_yy * Y;
    }
	
    return res;
}
