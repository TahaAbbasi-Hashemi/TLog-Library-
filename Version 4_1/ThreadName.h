#pragma once
#ifndef THREAD_NAME_H
#define THREAD_NAME_H

#include <map>
#include <string>
#include <thread>

/*
*	This uses the singleton desing pattern 
*	only one thread name should exist across the project. 
& This pattern is thread safe. 
*/

class ThreadName
{
private:
	int mValue;
	static ThreadName *s_mInstance;
	std::map<std::thread::id, std::string> names;

protected:
	ThreadName();

public:
	virtual ~ThreadName();
	bool addName(std::string newName);
	bool changeName(std::string newName);
	std::string getName(void);
	static ThreadName *instance(void);	// How to communicate with threadname.
};

#endif		// THREAD_NAME_H
