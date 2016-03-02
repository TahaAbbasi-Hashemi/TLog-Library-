#include "thread_names.h"

// Basic Functions
thread_names::thread_names(void) {}
thread_names::~thread_names(void) {}

// Public Functions
void thread_names::name_thread(string name)		{names.insert(make_pair(this_thread::get_id(), name));}
string thread_names::thread_name()				{return names[this_thread::get_id()];}



thread_names g_names;	// Global Variable