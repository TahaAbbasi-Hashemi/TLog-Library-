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

/*
	This is the severity level of the log 
	The levels are increase in severity from ALWAYS to CRITICAL
*/
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
/*
	This converts ESeverity into a string

	@param Severity This is the ESeverity the user wants tranformed into a string
	@return the string equivilent of the ESeverity given
*/
std::string ESeverity2String(ESeverity Serverity);

/*
	This is the lostruct that holds all the log information while it is stored in the queue
*/
struct Log
{
	std::string Date;		// The date at which log was made
	std::string Time;		// The exact time at which the log was made
	std::string Thread;		// The name of the thread which made the log 
	std::string FileName;	// The name of the file which made the log 
	ESeverity Severity;		// The severity level of the log
	int LineNumber;			// The line number of file which made the log
	std::string Message;	// The message the user wants to store
};

class Logging
{
private:
	bool mAlive;				// This shows the the logger is actively running
	bool mPaused;				// this shows that the logger is willing to process logs
	std::string mDirectory;		// This is the saving location of the logs
	std::string mLogFormat;		// This is the order of the variables in the log
	std::string mThreadName;	// This is thbe name of the thread processing the logs
	std::thread mThread;		// this is the thread dedicated for the WritingFunction
	std::vector<std::string> mSemiDirectory;	// This holds the semi direcotires
	std::vector<std::string> mFileNames;		// This holds the file names with in the directories
	std::queue<Log> mQueue;		// This is where all unprocesssed logs are stored

	/*
		This is the function which should only ever be used by mThread
		This is called in initalize and ends in terminate
		This function will end when mAlive is false
		The only source of input into this function is mQueue 
		The only source of ouput from this function is from a file. 
	*/
	void WritingFunction(void);	

	/*
		This takes a __FILE__ and finds the file name aspect instead of the entire directory as well 

		@param FileName This would be __FILE__
		@return This is the name of the file from __FILE__
	*/
	std::string getFileName(std::string FileName);
	std::string getDirectoryName(std::string FileName);
	std::string getTime(void);

public:
	Logging();
	virtual ~Logging();

	/*
		This adds a log to the queue

		@param Message The body of the log 
		@param FileName The file which the log was written in 
		@param LineNumber The line number which the log was written in 
		@param Severity the severity of the log. by default this is BASIC
	*/
	void ___mAddLine___(std::string Message, std::string FileName, int LineNumber, ESeverity Severity = BASIC);
	
	/*
		This called ___mAddLine___ and includes other information such as __FILE__ and __LINE__
		__LINE__ holds the line number at which the function is called 
		__FILE__ holds the file name at which the function is called 

		@param message This is the text line which will be saved in a log
		@param severity This holds the severity of the message the user wants logged
	*/
#define AddLine(message, severity) ___mAddLine___(message, __FILE__, __LINE__, severity);
	bool pause(void);
	bool resume(void);

	/*
		This starts the logging process 
		Before this function is called the program can still take logs 
		Only after this function will logs be processed

		@return true if the logging process started successfully, false if it is already started
	*/
	bool initalize(void);

	/* 
		This ends the logging process 
		After ths function is called the program can still take logs
		Logs will no longer be processed after this is called. 

		@return true if mThread is joined, false if it is noted yet started
	*/
	bool terminate(void);
	bool getAlive(void);
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

/*
	This converts a stirng into a wstring 
	wstring is used for windows functions so it is required

	@param & str	This is a reference to the string to be converted 
	@return This is the wstring version of the string
*/
std::wstring string2wstring(std::string & str);

/*
	This turns an integer into a string

	@param num This is the number that will be converted 
	@return this is the string version of the integer
*/
std::string int2string(int num);
#endif	// LOGGING_H

