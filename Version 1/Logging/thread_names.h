#pragma once

#include <iostream>
#include <map>
#include <string>
#include <thread>

using namespace std;
class thread_names
{
private:
	map<thread::id, string> names;
public:
	thread_names(void);
	~thread_names(void);

	void name_thread(string name);
	string thread_name();
};