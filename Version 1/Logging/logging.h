#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <fstream>
#include <ctime>
#include <sstream>
#include <vector>
#include <windows.h>
#include "thread_names.h"

using namespace std;
extern thread_names g_names;

enum severity_levels
{
	always,			// Most basic of a log
	basic,			// Means we are logging actual information veruses debugging logs
	info,			// Can mean anything other then something has happened
	debug,			// Means something has happened   - (good or bad)
	success,		// Means something good has happened
	warning,		// Means something bad might happen
	error,			// Means something bad has happened but will not crash the program
	critical		// Means something bad has happened and will crash the program
};


class logging
{
	/*
	This class is designed for taking input and outputting it into a file
	The program will work with 2 threads.
	One thread will exist only to remove the bottle neck of writing to file
	The only issue is there is a suttle delay when making the new thread

	Another issue is that there is a chance that there isnt enough room for
	all the threads. If the computer cant handle 2 threads then this logger
	might not be the best.

	The way this program works is that you create the function
	You can define the program, you dont have to initalize anything
	You can instantly start adding lines to the file.
	The only thing is nothing will actually be written unless you
	call the function init()
	This function will start the other thread to start writing to
	the file.
	*/
private:
	// Variable(s)
	// Threaded Variable(s) (variables that both threads use)
	bool alive;											// The variable decdiing if the thread should keep working
	queue<string> log_lines;						// The variable talking between threads
	thread log_thread;									// The thread that will be doing all the writing and everything.

	// Function(s)
	void thread_function(void);
	string date_time(void);

public:
	// Function(s)
	// Basic Function(s)
	logging(void);
	~logging(void);
	void init(void);								// The function to start the other thread.
	void kill(void);

	// Non-Basic function(s)
	void addline(string line_, severity_levels level_);		// Most commonly called function
};