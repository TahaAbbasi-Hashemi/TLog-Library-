#pragma once
#ifndef THREAD_NAME_H
#define THREAD_NAME_H

#include <map>
#include <string>
#include <thread>

class ThreadName
{
private:
	std::map<std::thread::id, std::string> names;

public:
	ThreadName();
	~ThreadName();
	bool addName(std::string newName);
	bool changeName(std::string newName);
	std::string getName(void);
};

#endif		// THREAD_NAME_H
