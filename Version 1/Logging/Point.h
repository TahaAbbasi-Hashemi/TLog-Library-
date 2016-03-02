#pragma once
#include <iostream>
#include <string>
#include <math.h>

using namespace std;
class Segment;

class Point
{
private:
	Point();
	// Functions
	// Variables
	const long origin_point = 0;    // This should never change
	long x; // Because these are  not const which means
	long y; // These can change.
	long z; // Remeber to take note of that. if it doesn't change it will be const
public:
	// Basics
	Point(long x_, long y_, long z_);
	~Point();
	// Within
	long long distance_from_point(const Point &given);
	long long distance_from_origin(void);

	// Returns
	// This is so dumb that I had -
	long inline get_x(void) { return x; }
	long inline get_x(void) const { return x; }
	long inline get_y(void) { return y; }
	long inline get_y(void) const { return y; }
	long inline get_z(void) { return z; }
	long inline get_z(void) const { return z; }
	long inline get_array(void) { return (x, y, z); }
	long inline get_array(void) const { return (x, y, z); }
};