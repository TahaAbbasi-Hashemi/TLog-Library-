#include "pch.h"
#include "ThreadName.h"

using namespace std;

ThreadName::ThreadName()
{
	mValue = 1;
}

ThreadName::~ThreadName()
{
	// Clearing memeory
	if (s_mInstance != 0)
	{
		delete s_mInstance;
	}
}

bool ThreadName::addName(string newName)
{
	// This function pairs the id of the thread to a actual name
	names.insert(make_pair(this_thread::get_id(), newName));
	return true;
}

string ThreadName::getName(void)
{
	// This function returns the name of the thread.
	return names[this_thread::get_id()];
}

ThreadName* ThreadName::s_mInstance = 0;

ThreadName * ThreadName::instance(void)
{
	// this is how the thread is communicated with.
	if (s_mInstance == 0)
	{
		s_mInstance = new ThreadName;
	}
	return s_mInstance;
}
