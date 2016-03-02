#include "thread_names.h"

thread_names::thread_names()
{
}
thread_names::~thread_names()
{
}
void thread_names::name_the_thread(string name)
{
	// This function pairs the id of the thread to a actual name
	names.insert(make_pair(this_thread::get_id(), name));
}
string thread_names::get_thread_name(void)
{
	// This function returns the name of the thread.
	return names[this_thread::get_id()];
}

// Creating A Global Variable
thread_names g_thread_names;