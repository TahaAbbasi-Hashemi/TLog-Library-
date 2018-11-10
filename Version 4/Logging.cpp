#include "pch.h"
#include "Logging.h"

string ESeverity2String(ESeverity Serverity)
{
	// This simply converts a ESeverity to a string
	string severity;

	switch (Serverity)
	{
	case ALWAYS:
		severity = "ALWAYS";
		break;
	case BASIC:
		severity = "BASIC";
		break;
	case INFO:
		severity = "INFO";
		break;
	case DEBUG:
		severity = "DEBUG";
		break;
	case SUCCESS:
		severity = "SUCCESS";
		break;
	case WARNING:
		severity = "WARNING";
		break;
	case CRITICAL:
		severity = "CRITICAL";
	}

	return severity;
}

Logging::Logging()
{
	mPaused = false;		// this means the logger is actively not processing logs
	mAlive = false;			// This means the logger is not initalized
	mDirectory = "RecordedLogs";
	mLogFormat = "%D %t %T %L %S : %M";	// The basic log format
	mThreadName = "LogProcessingThread";
	mSemiDirectory = { "Thread", "File", "Severity", "Everything" };
}

Logging::~Logging()
{
}

void Logging::WritingFunction(void)
{
	// This is the function being run only by a secondary thread. 
	// The purpose of the function is to take a log from the queue and put it in multiple files
	// The main source of input into this function is from the queue and its output is the files
	Log newLog;
	string tempDirectory;
	string tempFile;
	string tempLine = "";
	ofstream tempWrite;
	bool found;

	g_ThreadNames.addName(mThreadName);
	while (mAlive)
	{
		if (mPaused)
		{
			// If the program is paused, the code will skip processing
			continue;
		}	// pause if
		if (mQueue.empty())
		{
			// This is if the queue is fully empty there is no reason to do anythign
			continue;
		}	// Queue if 

		// Creating newLog 
		newLog = mQueue.front();
		mQueue.pop();	

		// Create Log to string
		for (unsigned int i = 0; i < mLogFormat.length(); i++)
		{
			if (mLogFormat[i] == '%')
			{
				found = true;
				continue;
			}

			if (found)
			{
				switch (mLogFormat[i])
				{
				case 'D':
					tempLine += newLog.Date;
					break;
				case 't':
					tempLine += newLog.Time;
					break;
				case 'T':
					tempLine += newLog.Thread;
					break;
				case 'S':
					tempLine += ESeverity2String(newLog.Severity);
					break;
				case 'L':
					tempLine += int2string(newLog.LineNumber);
					break;
				case 'M':
					tempLine += newLog.Message;
					break;
				}	// switch case
			}	// If found
			else
			{
				tempLine += mLogFormat[i];
			}
			found = false;
		}	// Log to string for loop

		// Creating directories 
		string test = "C:\\Users\\Taha Abbasi-Hashemi\\source\\repos\\Logging\\Logging";
		//tempDirectory = getDirectoryName(__FILE__)  + '\\' + mDirectory + '\\' + __DATE__;
		tempDirectory = test  + '\\' + mDirectory;
		CreateDirectory(string2wstring(tempDirectory).c_str(), NULL);
		tempDirectory += ('\\' + __DATE__);
		CreateDirectory(string2wstring(tempDirectory).c_str(), NULL);
		//mkdir(string2wstring(tempDirectory).c_str());
		for (auto const& location : mSemiDirectory)
		{
			string tempString2;
			tempString2 = tempDirectory + '\\' + location;
			CreateDirectory(string2wstring(tempString2).c_str(), NULL);
		}	// Creating directory for loop

		// Writing to file
		for (auto const& semi : mSemiDirectory)
		{
			if (semi == "Thread")
			{
				mFileNames.push_back(newLog.Thread);
				tempWrite.open(tempDirectory + '\\' + "Thread\\" + newLog.Thread + ".log");
				tempWrite << tempLine << '\n';
			}
			if (semi == "File")
			{
				mFileNames.push_back(newLog.FileName);
				tempWrite.open(tempDirectory + '\\' + "File\\" + newLog.FileName + ".log");
				tempWrite << tempLine << '\n';
			}
			if (semi == "Severity")
			{
				mFileNames.push_back(ESeverity2String(newLog.Severity));
				tempWrite.open(tempDirectory + '\\' + "Severity\\" + ESeverity2String(newLog.Severity) + ".log");
				tempWrite << tempLine << '\n';
			}
			tempWrite.close();
		}// Organizing fileNames for loop
		mSemiDirectory.push_back("Everything");
		tempWrite.open(tempDirectory + '\\' + "Everything/Everything.log");
		tempWrite << tempLine << '\n';
		tempWrite.close();

	}	// while loop
}

string Logging::getFileName(string FileName)
{
	// This takes ___FILE___ and breaks it down
	// __FILE__ provides both the directory plus the file name. 
	// This function will remove the directory and only return file name
	int index; // This is the location of the real file name

	index = -1;	// negative one is palce holder
	reverse(FileName.begin(), FileName.end());
	// Finds the seperastion from file name and directory location
	for (unsigned int i = 0; i < FileName.length(); i++)
	{
		if (FileName[i] == '\\')
		{
			if (index > 0)
			{
				continue;
			}
			index = i;
		}
	}
	// Returns something if no filename was found. 
	if (index == -1)
	{
		return "FileNameNotFound";
	}
	// Removes directory information from filename
	FileName.erase(index, FileName.length());
	reverse(FileName.begin(), FileName.end());

	return FileName;
}

string Logging::getDirectoryName(string FileName)
{
	// This takes ___FILE___ and breaks it down
	// __FILE__ provides both the directory plus the file name. 
	// This function will remove the file name and keep the directory
	int index; // This is the location of the real file name

	index = -1;	// negative one is palce holder
	// Finds the seperastion from file name and directory location
	for (unsigned int i = 0; i < FileName.length(); i++)
	{
		if (FileName[i] == '\\')
		{
			index = i;
		}
	}
	// Returns something if no filename was found. 
	if (index == -1)
	{
		return "FileNameNotFound";
	}
	// Removes directory information from filename
	FileName.erase(index, FileName.length());

	return FileName;
}

string Logging::getTime(void)
{
	// This returns the current time in string format
	string currentTime;
	char buffer[70];
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 70, "%X", &timeinfo);
	currentTime.append("");
	currentTime.append(buffer);
	currentTime.append("");

	return currentTime;
}

void Logging::___mAddLine___(string Message, string FileName, int LineNumber, ESeverity Severity)
{
	// This is the real AddLine function
	// This is where all the creation of Logs happen. 
	Log newLog;
	newLog.Date = __DATE__;
	newLog.Time = getTime();
	newLog.FileName = getFileName(FileName);
	newLog.Thread = g_ThreadNames.getName();
	newLog.Severity = Severity;
	newLog.LineNumber = LineNumber;
	newLog.Message = Message;
	mQueue.push(newLog);	// Saves the new log
}

bool Logging::pause(void)
{
	// This pauses the log processing process
	if (mPaused)
	{
		return false;
	}
	mPaused = true;
	return true;
}

bool Logging::resume(void)
{
	// This starts the log processing porcess
	if (mPaused)
	{
		mPaused = false;
		return true;
	}
	return false;
}

bool Logging::initalize(void)
{
	// This is used to start the logging process. 
	// Before this function the queue will take input
	// But no logging will happen. 
	if (mAlive)
	{
		// This tests to see  if the program has already started. 
		return false;
	}
	mPaused = false;
	mAlive = true;
	mThread = thread(&Logging::WritingFunction, this);
	return true;	// This means the the logger has succesfully started
}

bool Logging::terminate(void)
{
	// This kills the logging process. 
	// After this function no more logs will be processed.
	// Logs will  be accepted but nothing will be processed.
	mAlive = false;
	if (mThread.joinable() )
	{
		mThread.join();
		return true;
	}
	return false;
//	return true;	// This means that the logger has successfully stopped
}

bool Logging::getAline(void)
{
	// Returns the active state of the logger
	return mAlive;
}

bool Logging::getPaused(void)
{
	// Returns true if the logger is paused
	return mPaused;
}

bool Logging::getEmpty(void)
{
	// This returns true is the queue is empty and false to anything else
	if (mQueue.size() == 0)
	{
		return true;
	}
	return false;
}

string Logging::getDirectory(void)
{
	// This returns the saving directory of the logger
	return mDirectory;
}

string Logging::getLogFormat(void)
{
	// This returns the logging format of the logger
	return mLogFormat;
}

string Logging::getThreadName(void)
{
	// This returns the name of the thread processing logs 
	return mThreadName;
}

vector<string> Logging::getSemiDirectory(void)
{
	// This returns the SemiDirectory
	return mSemiDirectory;
}

bool Logging::changeDirectory(string newDirectory)
{
	// This allows for the program to change the saving Directory
	if (mPaused)
	{
		// This can only be run if there is no active logs being saved
		// Returns true is the directory was changed
		mDirectory = newDirectory;
		return true;
	}
	return false;	// False is returned if the directory did not change
}

bool Logging::changeLogFormat(string newLogFormat)
{
	// This allows for the program to change the format if the logger is paused
	if (mPaused)
	{
		// This can only be run if there is no active logs being saved
		// Returns true is the Logging format was changed
		mLogFormat = newLogFormat;
		return true;
	}
	return false;	// False is returned if nothing was changed
}

bool Logging::changeThreadName(string newThreadName)
{
	// This allows for the program to change the format if the logger is paused
	if (mPaused)
	{
		// This can only be run if there is no active logs being saved
		// Returns true is the threadname was changed
		mThreadName = newThreadName;
		return true;
	}
	return false;	// False is returned if nothing was changed
}

bool Logging::changeSemiDirectory(vector<string> newSemiDirectory)
{
	// This allows for the program to change the semiDirectory only if the logger is paused
	if (mPaused)
	{
		// This can only run if there are no active logs being saved
		// This returns true if the SemiDirectory is changed
		mSemiDirectory = newSemiDirectory;
		return true;
	}
	return false;
}

wstring string2wstring(string str)
{
	// converts a string to wide string or wstring.
	int len;
	int slength;

	slength = (int)str.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	wstring r(buf);

	delete[] buf;
	return r;
}

string int2string(int num)
{
	// This turns an integer into a string.
	stringstream ss;

	ss << num;

	return ss.str();
}
