/*
 * point_cloud.cpp
 *
 *  Created on: May 13, 2013
 *      Author: Thibaut Metivet
 */

#include "point_cloud.hpp"

using namespace LQCDA;

// -------------------------------------------------
// Point member functions
// -------------------------------------------------
inline double Point::x(size_t k)
{
    return x.at(k);
}
inline double Point::y(size_t k)
{
    return y.at(k);
}
inline double Point::xErr(size_t k)
{
    return xErr.at(k);
}
inline double Point::yErr(size_t k)
{
    return yErr.at(k);
}

// -------------------------------------------------
// PointCloud constructors and member functions
// -------------------------------------------------
PointCloud::PointCloud(const std::vector<Point>& points)
    : pts(points)
{
    
}

inline Point PointCloud::getPoint(size_t n)
{
    return pts.at(n);
}
