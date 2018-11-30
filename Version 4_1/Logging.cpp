#include "pch.h"
#include "Logging.h"

// This convertys ESeverity into a stirng
std::string ESeverity2String(ESeverity Serverity)
{
	std::string severity;

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

// This is the function which should only ever be used by mThread
// This is called in initalize and ends in terminate
// This function will end when mAlive is false
// The only source of input into this function is mQueue
// The only source of ouput from this function is from a file.
void Logging::WritingFunction(void)
{
	Log newLog;
	std::string tempDirectory;
	std::string tempFile;
	std::string tempLine = "";
	std::ofstream tempWrite;
	bool found;

	// This is how you communicate with singeltons
	ThreadName::instance() -> addName(mThreadName);
	while (mAlive)
	{
		if (mPaused)	// Pause if 
		{
			// If the program is paused, the code will skip processing
			continue;
		}	// pause if
		if (mQueue.empty())	// Queue if 
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

			if (found)	// if found
			{
				switch (mLogFormat[i])	// Switch case
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
			found = false;	// Resets the found
		}	// Log to string for loop

		// Creating directories 
		std::string test = "C:\\Users\\Taha Abbasi-Hashemi\\source\\repos\\Logging\\Logging";
		//tempDirectory = getDirectoryName(__FILE__)  + '\\' + mDirectory + '\\' + __DATE__;
		tempDirectory = test + '\\' + mDirectory;
		CreateDirectory(string2wstring(tempDirectory).c_str(), NULL);	// I am beyond lost why this is wrong
		tempDirectory += ('\\' + __DATE__);
		CreateDirectory(string2wstring(tempDirectory).c_str(), NULL);	// I am beyond lost why this is wrong
		//mkdir(string2wstring(tempDirectory).c_str());
		for (auto const& location : mSemiDirectory)
		{
			std::string tempString2;
			tempString2 = tempDirectory + '\\' + location;
			CreateDirectory(string2wstring(tempString2).c_str(), NULL);	// I am beyond lost why this is wrong
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

//This takes a __FILE__ and finds the file name aspect instead of the entire directory as well 
std::string Logging::getFileName(std::string FileName)
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

//This takes __FILE__ and returns the directory portion of the __FILE__
std::string Logging::getDirectoryName(std::string FileName)
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

// This reuturns the current time in a string format
std::string Logging::getTime(void)
{
	std::string currentTime;
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

// This adds a log to the queue
void Logging::___mAddLine___(std::string Message, std::string FileName, int LineNumber, ESeverity Severity)
{
	Log newLog;

	newLog.Date = __DATE__;
	newLog.Time = getTime();
	newLog.FileName = getFileName(FileName);
	newLog.Thread = ThreadName::instance() -> getName();	// Communication with singleton
	newLog.Severity = Severity;
	newLog.LineNumber = LineNumber;
	newLog.Message = Message;
	mQueue.push(newLog);	// Saves the new log
}

// Pauses the logging process
bool Logging::pause(void)
{
	if (mPaused)
	{
		return false;
	}
	mPaused = true;
	return true;
}

// This resumes the logging process
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

// This is used to start the logging process. 
// Before this function the queue will take input
// But no logging will happen. 
bool Logging::initalize(void)
{
	// This is to see if the logging process has already started
	if (mAlive)
	{
		return false;
	}
	// This is if the logging process has not started
	mPaused = false;
	mAlive = true;
	mThread = std::thread(&Logging::WritingFunction, this);
	return true;	// This means the the logger has succesfully started
}

// This ends the logging process
// After ths function is called the program can still take logs
// Logs will no longer be processed after this is called.
bool Logging::terminate(void)
{
	mAlive = false;
	// If the thread is able to be joined
	if (mThread.joinable())
	{
		mThread.join();
		return true;	// This means the thread has been joined and the logging process terminated
	}
	// If the treat is unable to be joined
	return false;		// The thread has yet to start or other error. 
}

// This finds out if the logging procvess has started
// true if it has been initalized
// false if it has not
bool Logging::getAlive(void)
{
	return mAlive;
}

// This finds out if the logging process is paused
bool Logging::getPaused(void)
{
	// Returns true if the logger is paused
	return mPaused;
}

//This finds out if the queue is currently empty
bool Logging::getEmpty(void)
{
	if (mQueue.size() == 0)
	{
		return true;	// If the queue is empty
	}
	return false;	// If the queue is not empty
}

//This returns the current directory location of where the logs will be saved
std::string Logging::getDirectory(void)
{
	return mDirectory;
}

// this returns the format of the log
std::string Logging::getLogFormat(void)
{
	return mLogFormat;
}

// This returns the name of the thread processing logs
std::string Logging::getThreadName(void)
{
	return mThreadName;
}

// This is used find the semi directories
// The semi directories are the folders inside the main directory
std::vector<std::string> Logging::getSemiDirectory(void)
{
	return mSemiDirectory;
}

bool Logging::changeDirectory(std::string newDirectory)
{
	// This allows for the program to change the saving Directory
	if (mPaused)
	{
		// This can only be run if there is no active logs being saved
		mDirectory = newDirectory;
		return true;	// if the directory was changed
	}
	return false;	// False is returned if the directory did not change
}

bool Logging::changeLogFormat(std::string newLogFormat)
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

bool Logging::changeThreadName(std::string newThreadName)
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

// This is used to change the semidriectory
// This does not add to but instead replaces the current semi directory
bool Logging::changeSemiDirectory(std::vector<std::string> newSemiDirectory)
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

//This converts a stirng into a wstring 
//wstring is used for windows functions so it is required
std::wstring string2wstring(std::string & str)
{
	// converts a string to wide string or wstring.
	int len;
	int slength;

	slength = (int)str.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	std::wstring r(buf);

	delete[] buf;
	return r;
}

// This turns an integer into a string
std::string int2string(int num)
{
	std::stringstream ss;

	ss << num;

	return ss.str();
}
