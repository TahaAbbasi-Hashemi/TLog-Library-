#include "pch.h"
#include "ThreadName.h"


ThreadName::ThreadName()
{
}

ThreadName::~ThreadName()
{
}

bool ThreadName::addName(std::string newName)
{
	// This function pairs the id of the thread to a actual name
	names.insert(make_pair(std::this_thread::get_id(), newName));
	return true;
}

std::string ThreadName::getName(void)
{
	// This function returns the name of the thread.
	return names[std::this_thread::get_id()];
}

// Creating a global variable 
ThreadName g_ThreadNames;
