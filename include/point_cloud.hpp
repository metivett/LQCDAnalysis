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
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> xErr;
    std::vector<double> yErr;

public:
    Point(const std::vector<double>& xx, const std::vector<double>& yy)
	: x(xx), y(yy), xErr(xx.size(),0.), yErr(yy.size(),0.)
    {}

    Point(const std::vector<double>& xx, const std::vector<double>& yy,
	  const std::vector<double>& xxErr, const std::vector<double>& yyErr)
	: x(xx), y(yy), xErr(xxErr), yErr(yyErr)
    {}

    std::vector<double> x() { return x; }
    std::vector<double> y() { return y; }
    std::vector<double> xErr() { return xErr; }
    std::vector<double> yErr() { return xErr; }
    
    double x(size_t k);
    double y(size_t k);
    double xErr(size_t k);
    double yErr(size_t k);
    
};

class PointCloud
{
private:
    std::vector<Point> pts;

public:
    PointCloud(const std::vector<Point>& points);

    size_t nPoints() { return pts.size(); }
    Point getPoint(size_t n);
};

} // namespace LQCDA

#endif	// POINT_CLOUD_HPP_
