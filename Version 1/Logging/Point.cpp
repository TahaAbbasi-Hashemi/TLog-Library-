#include "Point.h"


// Basics

Point::Point(long x_, long y_, long z_)
{
	x = x_;
	y = y_;
	z = z_;
}

Point::~Point()
{
}


long long Point::distance_from_point(const Point &given)
{
	long long distance;
	distance = sqrt(pow(x-given.get_x(), 2) + pow(y-given.get_y(), 2));
	return distance;
}


long long Point::distance_from_origin(void)
{
	long long distance;
	distance = sqrt(pow(x, 2) + pow(y, 2));
	return distance;
}