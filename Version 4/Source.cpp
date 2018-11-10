#include "pch.h"
#include <iostream>
#include "Logging.h"
#include "ThreadName.h"

// Global variables
extern ThreadName g_ThreadNames;

using namespace std;

int main()
{
	g_ThreadNames.addName("MainThread");
	Logging logger;
	logger.AddLine("This is a line", BASIC);
	logger.initalize();
	logger.terminate();
	string test;
	getline(cin, test);
}
