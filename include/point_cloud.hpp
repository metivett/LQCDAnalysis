/*
 * point_cloud.hpp
 *
 *  Created on: May 13, 2013
 *      Author: Thibaut Metivet
 */

#ifndef POINT_CLOUD_HPP_
#define POINT_CLOUD_HPP_

#include <vector>

namespace LQCDA {

class Point
{
private:
    std::vector<double> _X;
    std::vector<double> _Y;

public:
    Point(const std::vector<double>& xx, const std::vector<double>& yy) :
	_X(xx), _Y(yy)
    {}

    std::vector<double> X() { return _X; }
    std::vector<double> Y() { return _Y; }
    
    double X(size_t k);
    double Y(size_t k);
    
};

class PointCloud
{
private:
    std::vector<Point> _Points;

    // Covariance matrices
    Eigen::MatrixXd _C_yy;
    Eigen::MatrixXd _C_xx;
    Eigen::MatrixXd _C_xy;

public:
    PointCloud(const std::vector<Point>& points
	       const Eigen::MatrixXd& C_yy,
	       const Eigen::MatrixXd& C_xx,
	       const Eigen::MatrixXd& C_xy);

    size_t NbOfPoints() { return _Points.size(); }
    Point GetPoint(size_t n);
};

} // namespace LQCDA

#endif	// POINT_CLOUD_HPP_
