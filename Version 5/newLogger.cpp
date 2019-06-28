#include "newLogger.hpp"

newLogger* newLogger::sm_Instance = 0;

newLogger::newLogger()
{
    sm_Instance = 0;
    m_occured = false;
    m_active = false;
    m_directory = "RecordedLogs";
    m_logFormat = "%D %t %T %L %S : %M";
    m_threadName = "LogProcessingThread";
    logFileName = "RecordedLogs.log";
}

newLogger * newLogger::instance(void)
{
    if (sm_Instance == 0)
    {
        sm_Instance = new newLogger;
    }
    return sm_Instance;
}

newLogger::~newLogger()
{
    // End Threading process first
    terminate();

    // Then delete memory
    if (sm_Instance != 0)
    {
        delete sm_Instance;
    }
}

bool newLogger::setThreadName_(std::string name)
{
    try
    {
        threadNameLock.lock();
        m_names.insert(std::make_pair(std::this_thread::get_id(), name));
        threadNameLock.unlock();
    }
    catch (std::exception & e)
    {
        return false;
    }
    return true;
}

std::string newLogger::getThreadName(void)
{
    std::string returnName;

    threadNameLock.lock();
    returnName = m_names [std::this_thread::get_id()];
    threadNameLock.unlock();

    return returnName;
}

void newLogger::__mNewLine__(std::string message, std::string fileName, int lineNumber, ESeverity severity)
{
    SLog newLog;

    newLog.Date = __DATE__;
    newLog.Time = getStringTime();
    newLog.FileName = findFileName(fileName);
    newLog.Thread = getThreadName();
    newLog.Severity = severity;
    newLog.LineNumber = lineNumber;
    newLog.Message = message;
    
    queueLock.lock();
    m_logLines.push(newLog);
    queueLock.unlock();
}

bool newLogger::pause(void)
{
    if (m_paused)
    {
        return false;
    }
    m_paused = true;
    return false;
}

bool newLogger::resume(void)
{
    if (!m_paused)
    {
        return false;
    }
    m_paused = true;
    return false;
}

std::string newLogger::findFileName(std::string fileDirectory)
{
    int index;

    index = -1;
    reverse(fileDirectory.begin(), fileDirectory.end());

    for (unsigned int i = 0; i < fileDirectory.length(); i++)
    {
        if (fileDirectory [i] == '\\')
        {
            index = i;
            break;
        }
    }
    // If it couldn't find seperate file name from directory
    if (index == -1)
    {
        return "";
    }
    // Removes directory information from filename
    fileDirectory.erase(index, fileDirectory.length());
    reverse(fileDirectory.begin(), fileDirectory.end());

    return std::string();
}

std::string newLogger::findDirectoryName(std::string fileDirectory)
{
    int index; // This is the location of the real file name

    index = -1;
    reverse(fileDirectory.begin(), fileDirectory.end());
    for (unsigned int i = 0; i < fileDirectory.length(); i++)
    {
        if (fileDirectory [i] == '\\')
        {
            index = i;
            break;
        }
    }
    // Returns something if no fileDirectory was found. 
    if (index == -1)
    {
        return "";
    }
    // Removes filename information from file directory
    fileDirectory.erase(0, index);
    reverse(fileDirectory.begin(), fileDirectory.end());

    return fileDirectory;
}

std::string newLogger::getStringTime(void)
{
    std::string currentTime;
    char buffer [70];
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

std::string newLogger::ESeverity2String(ESeverity severity)
{
    std::string stringSeverity;

    switch (severity)
    {
        case ALWAYS:
            stringSeverity = "ALWAYS";
            break;
        case BASIC:
            stringSeverity = "BASIC";
            break;
        case INFO:
            stringSeverity = "INFO";
            break;
        case DEBUG:
            stringSeverity = "DEBUG";
            break;
        case SUCCESS:
            stringSeverity = "SUCCESS";
            break;
        case WARNING:
            stringSeverity = "WARNING";
            break;
        case CRITICAL:
            stringSeverity = "CRITICAL";
    }

    return stringSeverity;
}

std::string newLogger::int2string(int integer)
{
    std::stringstream ss;

    ss << integer;

    return ss.str();
}

void newLogger::threadFunction(void)
{
    SLog newLog;
    std::string tempDirectory;
    std::string tempFile;
    std::string writtenLog;
    std::ofstream tempWrite;
    bool found;

    setThreadName_(m_threadName);
    while (m_active)
    {
        if (m_paused)
        {
            continue;
        }
        if (m_logLines.empty())
        {
            continue;
        }
        if (m_flush)
        {
            tempWrite.open(logFileName, std::ofstream::out | std::ofstream::trunc);
            tempWrite.close();
            m_flush = false;
        }

        // Read log from queue
        queueLock.lock();
        newLog = m_logLines.front();
        m_logLines.pop();
        queueLock.unlock();

        writtenLog = "";
        // Adding date, time, threadname, filename and so on into their respective locations.
        for (unsigned int i = 0; i < m_logFormat.length(); i++)
        {
            if (m_logFormat [i] == '%')
            {
                found = true;
                continue;
            }

            if (found)
            {
                switch (m_logFormat [i])
                {
                    case 'D':
                        writtenLog += newLog.Date;
                        break;
                    case 't':
                        writtenLog += newLog.Time;
                        break;
                    case 'T':
                        writtenLog += newLog.Thread;
                        break;
                    case 'S':
                        writtenLog += ESeverity2String(newLog.Severity);
                        break;
                    case 'L':
                        writtenLog += int2string(newLog.LineNumber);
                        break;
                    case 'M':
                        writtenLog += newLog.Message;
                        break;
                }
                found = false;
            }
            else
            {
                writtenLog += m_logFormat [i];
            }
        }

        tempWrite.open(logFileName, std::ofstream::out | std::ofstream::app);
        tempWrite << writtenLog << '\n';
        tempWrite.close();

    }
}

bool newLogger::init(void)
{
    if (m_occured)
    {
        return false;
    }
    m_occured = true;
    if (m_active)
    {
        return false;
    }
    m_active = true;
    //Sleep(10);
    m_loggingThread = std::thread(&newLogger::threadFunction, this);
    return false;
}

bool newLogger::terminate(void)
{
    if (!m_occured)
    {
        return false;
    }
    if (!m_active)
    {
        return false;
    }

    m_active = false;
    m_paused = true;
    //Sleep(10);
    if (m_loggingThread.joinable())
    {
        m_loggingThread.join();
        return true;
    }

    return false;
}

bool newLogger::changeDirectory(std::string newDirectory)
{
    return false;
}

bool newLogger::changeLogFormat(std::string newLogFormat)
{
    m_logFormat = newLogFormat;
    return true;
}

bool newLogger::changeThreadName(std::string newThreadName)
{
    return false;
}

bool newLogger::flush(void)
{
    m_flush = true;
    return true;
}
