#include "pch.h"
#include <iostream>

#include "Logging.h"
#include "ThreadName.h"

int main()
{
	ThreadName::instance() -> addName("MainThread");  // Creates the name for the thread
	Logging logger;
	for (int i = 0 ; i < 100; i++)
	{
		logger.AddLine("This is a line", BASIC);	// Creates a log
	}
	logger.initalize();	// Starts the logging process

	logger.pause();	// Pauses the logging process
	logger.resume();	// Resumes the logging process

	logger.terminate();	// Ends the logging process and has the thread end.
	std::cout << "the program has finished and is waiting for a enter press to close" << std::endl;
	std::string test;
	std::getline(std::cin, test);
}
