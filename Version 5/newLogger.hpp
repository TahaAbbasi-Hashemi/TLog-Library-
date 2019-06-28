#pragma once
#ifndef NEWLOGGER_HPP
#define NEWLOGGER_HPP

#include <map>
#include <vector>
#include <string>
#include <queue>
#include <thread>
#include <fstream> 
#include <sstream>
#include <mutex>

//! Used to house the severity or importance of all the logs
/*!
This is a enumeration class used to indicate all the different possible severity/importance levels of the logs.
For more information regarding the severity please refer to the enumerated values. 
*/
enum ESeverity
{
    //! This means this is a log that will always show up during the loggnig process
    ALWAYS,
    //! This is a basic log
    BASIC,
    //! This is a information log, this holds value information regarding the program
    INFO,
    //! This is a log used for debugging the program
    DEBUG,
    //! This is a log used for indicating when something has been successful
    SUCCESS,
    //! This is a log used to indicate that something may have gone astray but will not crash the program
    WARNING,
    //! This is a log used to indicate the fatal death of the program.
    CRITICAL
};

//! This is the general structure of a log used during the logging process. 
/*!
For more information regarding SLog please refer to each of the variables inside the log.
*/
struct SLog
{
    //! The date at which log was made; Day, Month, Year format. 
    std::string Date;
    //! The exact time at which the log was made; Hour, minuite, second. 
    std::string Time;
    //! The name of the thread that produced the log. 
    std::string Thread;
    //! The name of the file that the log was made in.
    std::string FileName;
    //! This is used to indicate the severity or importance of the log.
    ESeverity Severity;
    //! This is the exact line number that the log was written on. 
    int LineNumber;
    //! This is the message that the user wants to store in the log.
    std::string Message;
};

//! This is a singleton class used to allow for logging within a program
/*!
The class would hold and maintain its own thread and ensure that its values are all thread safe. 
The class would continuously save the documents it is using just to make sure that in a moment of a crash that the logs would be saved. 
The class is able to write logs in a dedicated directory. 
In order to ensrue that each log will be saved the logger opens and then closes each file each time it wants to write to a file. 
If you have too many logs being added to the queue at a time the logger may not be able to keep up. 
If you have to terminate the logger make sure to give the logger a few miliseonds to ensure that everything is written. 
*/
class newLogger
{
public:
    //! Constructor for singleton class
    /*!
    This is how you interact with a singleton class. 
    Calling this function lets you do anything with the class. 
    */
    static newLogger *instance(void);

    //! Destructor. 
    /*!
    If you failed to terminate the program before destructor is called it will terminate for you.
    */
    virtual ~newLogger();
    
    //! Connects a thread ID to a name
    /*!
    The function asks for the thread calling the functino for its name,
    It then takes the paramater name and assosiates it with the name
    This assosiation takes place in a map
    /param name This is the name of the thread.
    */
    bool setThreadName_(std::string name);

    //! Returns the name of the thread
    /*!
    The function asks for the thread calling the function for its ID,
    It then looks for what the name is assosiated with and returns the string assosiated with the ID
    This assosiation takes place in a map
    \return A string value holding the name of the thread.
    */
    std::string getThreadName(void);

    //! The function for adding a log to the queue. DO NOTE CALL!
    /*!
    Because hundreds of logs get added at the same time
    This function deals with adding those logs to a queue to be processed and stored as logs.
    This call should not be called, instead addLine should be called instead
    \param message This is the message the user wants to store 
    \param fileName this is the name of the file the log was created.
    \param lineNumber This is the line number of where the log was created. 
    \param severity This is the severity of the log, depending on severity the log can or can not be made.
    */
    void __mNewLine__(std::string message, std::string fileName, int lineNumber, ESeverity severity);
    #define addLine(message, severity) __mNewLine__(message, __FILE__, __LINE__, severity);

    //! Pauses the logging process
    /*!
    \return returns true if the process is paused, and false if it is already paused.
    */
    bool pause(void);

    //! Resumes the logging process
    /*!
    \return Returns true if the process is resumed and false if it was not paused.
    */
    bool resume(void);

    //! Starts the logging process
    /*!
    This function can only be called once, if it has been called before it will do nothing
    Once this functino nothing can be changed until the program is pasued.
    \return returns true, if the logging process has started and false if it did not. Returning false means it has already been started before.
    */
    bool init(void);

    //! This ends the logging process. 
    /*!
    This function can only be called once, if it has been called it will only return false afterwards
    This function can not be called before init() if it has been called before init() it will return false
    If the logging process can not be stopped it will return false.
    \return This returns true if the logging process has stopped, it will return false, if the logging process has already ended or if it can not be killed.
    */
    bool terminate(void);

    //! Changes the directory that the program will save the log files in.
    /*!
    \param newDirectory This is the new directory location without '\\' the end.
    \return If the new directory is suitable then it will return true, false otherwise.
    */
    bool changeDirectory(std::string newDirectory);

    //! Changes the format that the logs will be saved in 
    /*!
    \param newLogFormat This is the new format the logs will be saved in
    \return If the newformat is suitable to be saved it will return true, false otherwise.
    */
    bool changeLogFormat(std::string newLogFormat);

    //! Changes the name of the logging thread
    /*!
    \param this is the new name of the logging thread
    \return will return true if it has been changed, and false if the name was not suitable to be used.
    */
    bool changeThreadName(std::string newThreadName);

    //! Cleans the log file of any logs. 
    /*!
    This cleans the log file of any logs by opening the log file without doing append 
    This would then remove all properties in the log essnetially cleaning it. 
    */
    bool flush(void);

private:
    //! The instance of the class. 
    static newLogger *sm_Instance;

    //! Private Constructor for a singleton class.
    newLogger();

    //! Meant to stop the compiler form being able to create new instances
    /*! 
    This is intentionally not implemnted anywhere to ensure that the compiler knows not to maky another instance of the logger.
    */
    newLogger(newLogger const& copy);

    //! This is meant to stop the compiler from being able to copy information about the instance
    /*!
    This is intentinoally not implemented anywhere to ensure that no matter what the compiiler wont copy anything. 
    */
    newLogger& operator=(newLogger const& copy);

protected:
    //! This is used for pairing thread names with thread id
    std::map<std::thread::id, std::string> m_names;

    //! This is to prevent double initaliztion of the program. 
    bool m_occured;

    //! This is to show that it has been initalized
    bool m_active;

    //! This is to show if the program is paused or not
    bool m_paused;

    //! This is for when the logging thread is going to flush the contents of the logs 
    bool m_flush;

    //! to ensure that the queue is thread safe
    std::mutex queueLock;

    //! To ensure to thread names variable is thread safe. 
    std::mutex threadNameLock;
    
    //! This is where the files will be saved 
    std::string logFileName;

    //! This is the queue where all the logs are stored
    std::queue<SLog> m_logLines;

    //! This is the thread that writes all the logs in during the logging process
    //! This thread also has its own function. "threadFunction"
    std::thread m_loggingThread;

    //! This is the directory that the logs will be saved in, this is optional
    std::string m_directory;

    //! This is the format that the logs will be saved in, changing this is optional.
    std::string m_logFormat;

    //! This is the name of the thread doing the logging process. 
    //! changing this is optional. 
    std::string m_threadName;

    //! Takes __FILE__ and returns the file name portion of it at the end 
    /*!
    __FILE__ holds the name of the file and the name of the directory
    The purpose of the function is to seperate the file name with the directory
    A example of what __FILE__ provides is as such "C:folderName\\fileName.txt"
    This function returns "fileName.txt"
    \param fileDirectory This is the full location of the file name
    \return the name of the file without its directory.
    */
    std::string findFileName(std::string fileDirectory);

    //! Takes __FILE__ and returns the file directory portion of it
    /*!
    __FILE__ holds the name of the file and the name of the directory
    The purpose of the function is to seperate the file name with the directory
    A example of what __FILE__ provides is as such "C:folderName\\fileName.txt"
    This function returns "C:folderName"
    \param fileDirectory This is the full location of the file name
    \return the name of the directory name without the file name.
    */
    std::string findDirectoryName(std::string fileDirectory);

    //! Returns the current time in a string format
    std::string getStringTime(void);

    //! Returns a string corresponding the the severity of the log.
    std::string ESeverity2String(ESeverity serverity);

    //! Converts a integer to a string. 
    std::string int2string(int integer);

    //! This is the where the logging process occurs 
    /*!
    This function is meant to be used only by a single thread. 
    This should only be called once and will be called only when the init function is called
    This function goes through the queue of logs and processes each one. 
    Depending on variables set it can choose to either process the logs or skip them. 
    The function would end its self once terminate is called. 
    If terminate is not called than the function will be killed during the destructor call. 
    */
    void threadFunction(void);

};

//! Creates a log
#define makeLog(message, severity) newLogger::instance()->addLine(message, severity);
//! Changes the name of the thread
#define setThreadName(name) newLogger::instance()->setThreadName_(name);
//! Changes the format of the logs
#define changeLoggerFormat(newLogFormat) newLogger::instance()->changeLogFormat(newLogFormat);
//! Removes all logs already saved
#define flushLogs() newLogger::instance()->flush();
//! Starts the logging process
#define startLogging() newLogger::instance()->init();
//! Pauses the logging process
#define pauseLogger() newLogger::instance()->pause();
//! Resumes the logging process
#define resumeLogger() newLogger::instance()->resume();
//! Stops the loggin process
#define endLogger() newLogger::instance()->terminate();

#endif // !NEWLOGGER_HPP