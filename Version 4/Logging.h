#pragma once
#ifndef LOGGING_H
#define LOGGING_H

#include "ThreadName.h"
//#include <chrono>
#include <ctime> 
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>
#include <Windows.h>

// Global variables
extern ThreadName g_ThreadNames;

enum ESeverity
{
	ALWAYS,			// Logs that will always be there
	BASIC,			// Very basic logs
	INFO,			// Information holding logs
	DEBUG,			// Logs holding information for debugging the program
	SUCCESS,		// Logs indicating something has successfully happened
	WARNING,		// Logs showing something may or may not go wrong
	CRITICAL		// Logs showing fatal death of the program
};
string ESeverity2String(ESeverity Serverity);

struct Log
{
	string Date;
	string Time;
	string Thread;
	string FileName;
	ESeverity Severity;
	int LineNumber;
	string Message;
};

class Logging
{
private:
	bool mAlive;		// This shows the the logger is actively running
	bool mPaused;		// this shows that the logger is willing to process logs
	string mDirectory;	// This is the saving location of the logs
	string mLogFormat;	// This is the order of the variables in the log
	string mThreadName;	// This is thbe name of the thread processing the logs
	thread mThread;
	vector<string> mSemiDirectory;	// This holds the semi direcotires
	vector<string> mFileNames;		// This holds the file names with in the directories
	queue<Log> mQueue;	// This is where all unprocesssed logs are stored

	void WritingFunction(void);		// This is the fucntion being run by the created thread
	string getFileName(string FileName);
	string getDirectoryName(string FileName);
	string getTime(void);

public:
	Logging();
	~Logging();
	void ___mAddLine___(string Message, string FileName, int LineNumber, ESeverity Severity = BASIC);
#define AddLine(message, severity) ___mAddLine___(message, __FILE__, __LINE__, severity);
	bool pause(void);
	bool resume(void);
	bool initalize(void);
	bool terminate(void);
	bool getAline(void);
	bool getPaused(void);
	bool getEmpty(void);
	string getDirectory(void);
	string getLogFormat(void);
	string getThreadName(void);
	vector<string> getSemiDirectory(void);
	bool changeDirectory(string newDirectory);
	bool changeLogFormat(string newLogFormat);
	bool changeThreadName(string newThreadName);
	bool changeSemiDirectory(vector<string> newSemiDirectory);
};

wstring string2wstring(string str);
string int2string(int num);
#endif	// LOGGING_H

