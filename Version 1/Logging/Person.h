#pragma once
#include <iostream>
#include <string>
#include <queue>

using namespace std;

class Person
{
private:
	string name;
	long long run;
	int fastest;
	int previous;

public:
	Person();
	~Person();
};

