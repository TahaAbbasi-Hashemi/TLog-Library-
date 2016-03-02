#pragma once
#include <iostream>
#include <string>
#include "Point.h"
using namespace std;

class Circle
{
private:
	Circle();
	~Circle();
	// Functions
	// Variables
	long radius;
public:
	// Basics
	Circle(Point center_, long radius_);
	// Within
	bool within(const Point &given);
	bool on(const Point &given);
	// Returns
	double get_slope(void);
	long get_y(void);
	string get_equation(void);  // (x-origin_x)^2 + (y-origin_y)^2 = r^2
};