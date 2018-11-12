#pragma once
#ifndef LOGGING_H
#define LOGGING_H

#include "ThreadName.h"
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
std::string ESeverity2String(ESeverity Serverity);

struct Log
{
	std::string Date;
	std::string Time;
	std::string Thread;
	std::string FileName;
	ESeverity Severity;
	int LineNumber;
	std::string Message;
};

class Logging
{
private:
	bool mAlive;		// This shows the the logger is actively running
	bool mPaused;		// this shows that the logger is willing to process logs
	std::string mDirectory;	// This is the saving location of the logs
	std::string mLogFormat;	// This is the order of the variables in the log
	std::string mThreadName;	// This is thbe name of the thread processing the logs
	std::thread mThread;
	std::vector<std::string> mSemiDirectory;	// This holds the semi direcotires
	std::vector<std::string> mFileNames;		// This holds the file names with in the directories
	std::queue<Log> mQueue;	// This is where all unprocesssed logs are stored

	void WritingFunction(void);		// This is the fucntion being run by the created thread
	std::string getFileName(std::string FileName);
	std::string getDirectoryName(std::string FileName);
	std::string getTime(void);

public:
	Logging();
	~Logging();
	void ___qwerty___(std::string Message, std::string FileName, int LineNumber, ESeverity Severity = BASIC);
#define AddLine(message, severity) ___qwerty___(message, __FILE__, __LINE__, severity);
	bool pause(void);	// Pauses the logging process
	bool resume(void);	// Resumses the logging process
	bool initalize(void);
	bool terminate(void);
	bool getAline(void);
	bool getPaused(void);
	bool getEmpty(void);
	std::string getDirectory(void);
	std::string getLogFormat(void);
	std::string getThreadName(void);
	std::vector<std::string> getSemiDirectory(void);
	bool changeDirectory(std::string newDirectory);
	bool changeLogFormat(std::string newLogFormat);
	bool changeThreadName(std::string newThreadName);
	bool changeSemiDirectory(std::vector<std::string> newSemiDirectory);
};

std::wstring string2wstring(std::string & str);
std::string int2string(int num);
#endif	// LOGGING_H

