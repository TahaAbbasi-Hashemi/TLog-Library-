#include "pch.h"
#include "ThreadName.h"

using namespace std;

ThreadName::ThreadName()
{
}

ThreadName::~ThreadName()
{
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

// Creating a global variable 
ThreadName g_ThreadNames;
