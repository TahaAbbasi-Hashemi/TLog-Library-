#pragma once
#include <iostream>
#include <string>
#include "Point.h"
using namespace std;

class Segment
{
private:
	Segment();
	~Segment();
	// Functions
	// Variables
	double slope;
	long y_intercept;
public:
	// Basics
	Segment(int slope_, long y_intercept_);
	// Within
	bool above(const Point &given);
	bool below(const Point &given);
	bool on(const Point &given);
	// Returns
	double inline get_slope(void) { return slope; }
	long inline get_y(void) { return y_intercept; }
	string inline get_equation(void) { return "Blank Equation for now."; }
};