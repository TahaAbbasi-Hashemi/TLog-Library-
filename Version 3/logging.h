#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <chrono>
#include "thread_names.h"
using namespace std;
extern thread_names g_thread_names;

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
//------------------- Variables
	bool alive = true;
	bool occured = false;
	string directory_location = "Recorded Logs";
	queue<string> log_queue;
	thread writing_thread;
	string thread_name = "Writing Thread";
	string log_format = "%D %T %F %L %S : %M";
                        // %D = Date and Time
                        // %T = Thread Name
                        // %F = File Name-
                        // %S = Severity
                        // %L = Line Number
                        // %M = Message
	char between_variable = 'œ';
    string null_string = "œ";
	string previous_date = "None";
	vector<string> file_keys;
	vector<ofstream> file_values;
    unsigned int maxium_compressed_string_length = 0;
	vector<string> shortened_strings;	// This is where each set of characters is stored that will be shortened.

//------------------- Functions
	void writing_function(void);
	string get_date_and_time(void);
	string file_name_only(string file_location);
	string int_to_string(long long number);
	vector<string> find_order(void);

public:
	logging();
	~logging();
	void init(void);			// Starts the logging process
	void kill(void);			// Ends the logging process
	void __addline__(string line, string file_name, int line_numbet, severity_levels level=basic);
	#define addline(line, level) __addline__(line, __FILE__, __LINE__, level)	// This is the one you want to call.
	void change_format(string new_format);
	void change_thread_name(string new_name);
	void change_directory(string new_directory);
	void change_compression(bool is_compression_true);
};

