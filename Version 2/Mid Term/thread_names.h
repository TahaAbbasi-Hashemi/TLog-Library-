#pragma once
#include <iostream>
#include <map>
#include <string>
#include <thread>
using namespace std;

//The purpose of this class is to sort and organize threads.
class thread_names
{
private:
	map<thread::id, string> names;
public:
	thread_names();
	~thread_names();
	void name_the_thread(string name);
	string get_thread_name(void);
};