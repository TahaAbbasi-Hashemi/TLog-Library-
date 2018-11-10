#pragma once
#ifndef THREAD_NAME_H
#define THREAD_NAME_H

#include <map>
#include <string>
#include <thread>

using namespace std;

class ThreadName
{
private:
	map<thread::id, string> names;

public:
	ThreadName();
	~ThreadName();
	bool addName(string newName);
	bool changeName(string newName);
	string getName(void);
};

#endif		// THREAD_NAME_H
