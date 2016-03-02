#include "logging.h"


// Basic Functions
logging::logging() { alive = true; }
logging::~logging(void) {}


// Private Functions

void logging::thread_function(void)
{
	/*
	The purpose of this function is to hold a single thread
	This thread will have the job of simply writing stuff to storage
	It will also have total and utter control of the logs
	The only way to access this thread is through the two variables
	The first one is working.
	This decides if this function should live or die.
	The second one is log_lines, this is the queue where all the lines are saved.
	*/


	// Global Variable things
	g_names.name_thread("Logging Thread");

	// Creating needed directories
	string main_folder = "Recorded Logs";
	CreateDirectory(main_folder.c_str(), NULL);

	string previous_date = "None";
	vector<string> file_keys;
	vector<ofstream> file_values;


	// Thread loop
	while (alive || !log_lines.empty())
	{
		if (log_lines.empty()) { continue; }	// If there is nothing in the queue, just kindof chill for a while.
		string log_date;
		bool inital = true;
		string log_thread;
		string log_severity_level;

		string line = log_lines.front();
		string reading = "None";

		// This loop here isolates the groups.
		// It takes it from all variables that are currently known
		for (unsigned int index = 0; index < line.size(); index++)
		{
			/*
			What this entire for loop does, is it detects where the log is coming from and creates variables to go along with its location. 
			Now that we would have its location, what happens now is that store the info.
			And get ready to start logging it in the right places.

			Basically what this does, is it goes from character to character detecting what the character is and if it
			has one of ther wanted character values that we are looking for and stores the other charactes based on it.
			Try reading the code below for a bit more info.
			*/
			char character = line[index];	// The character that will be going through the search detector
			
			switch (character)
			{
				case ('\'') :	// Date
				{
					if (inital == true) 
					{ 
						reading = "date";
						inital = false;
					}
					else { reading = "None"; }
					break;
				}
				case ('[') :	// Thread Name
				{
					reading = "thread name";
					break;
				}
				case (']') :
				{
					reading = "None";
					break;
				}
				case ('<') :	// Severity level
				{
					reading = "security level";
					break;
				}
				case ('>') :
				{
					reading = "None";
					break;
				}
			}

			// storing the characters that were acceoted into a string.
			if (reading == "date")
			{ 
				if (character != '\'') { log_date += character; }
			}
			else if (reading == "thread name")
			{
				if (character != '[') { log_thread += character; }
			}
			else if (reading == "security level")
			{
				if (character != '<') { log_severity_level += character; }
			}
		}

		// Because Dates Can't Go Backwards, the moment we have a new date, we stop using the old date
		if (log_date != previous_date)
		{
			previous_date = log_date;
			string date_folder = main_folder + '\\' + log_date;
			CreateDirectory(date_folder.c_str(), NULL);


			string thread_folder = main_folder + '\\' + log_date + "/Threads";
			string level_folder = main_folder + '\\' + log_date + "/Severity levels";
			CreateDirectory(thread_folder.c_str(), NULL);
			CreateDirectory(level_folder.c_str(), NULL);

			if (file_keys.size() != 0)
			{
				file_keys.clear();
				for (unsigned int index = 0; index < file_values.size(); index++) { file_values[index].close(); }
				file_values.clear();
			}

			// Everything file should always be number 0
			file_keys.push_back("Everything");
			ofstream everything_file(main_folder + '/' + log_date + "/Everything.log", ios::app);
			file_values.push_back(move(everything_file));

		}
		
		// Saving thread file, and writing to thread documents.
		{
			bool thread_is = false;
			int my_index = 0;
			for (unsigned int index = 0; index < file_keys.size(); index++)
			{
				if (file_keys[index] == log_thread)
				{
					thread_is = true;
					my_index = index;
				}
			}
			if (!thread_is)
			{
				file_keys.push_back(log_thread);
				ofstream thread_file(main_folder + '/' + log_date + "/Threads/" + log_thread + ".log", ios::app);
				file_values.push_back(move(thread_file));
				my_index = file_keys.size() - 1;
			}
			file_values[my_index] << line;
		}

		// Saving severity level file, and writing to said documents.
		{
			bool level_is = false;
			int my_index = 0;
			for (unsigned int index = 0; index < file_keys.size(); index++)
			{
				if (file_keys[index] == log_severity_level)
				{
					level_is = true;
					my_index = index;
				}
			}
			if (!level_is)
			{
				file_keys.push_back(log_severity_level);
				ofstream level_file(main_folder + '/' + log_date + "/Severity Levels/" + log_severity_level + ".log", ios::app);
				file_values.push_back(move(level_file));
				my_index = file_keys.size() - 1;
			}
			file_values[my_index] << line;
		}

		file_values[0] << line;	// This is where we write to the everything file.

		log_lines.pop();
	}

	// Final cleaning, before the program closes.
	if (file_keys.size() != 0)
	{
		file_keys.clear();
		for (unsigned int index = 0; index < file_values.size(); index++) { file_values[index].close(); }
		file_values.clear();
	}
}


// Public Functions
void logging::init(void)
{
	static bool occured = false;
	if (occured) { return; }
	log_thread = thread(&logging::thread_function, this);
}

void logging::kill(void)
{
	alive = false;
	log_thread.join();
}

string logging::date_time()
{
	/*
	The purpose of this function is to make a string with the current date
	and time.
	the format is very simple. it just the year, month, day. Then hour minite second.
	*/
	string final_ = "'";
	final_.append(__DATE__);
	final_.append("' ");

	char buffer[70];		// The variable that will hold the current date and time
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 70, "%X", &timeinfo);

	final_.append("");
	final_.append(buffer);
	final_.append("");
	return final_;
}

void logging::addline(string line_, severity_levels level_)
{
	// String stuff
	std::string _line;
	_line.reserve(300);	// if it is greater then 300 characters, we are in bad luck.

	//Date and Time
	_line.append(date_time());
	_line.append(" ");

	// Thread ID Name
	string name = g_names.thread_name();

	_line.append("[");
	_line.append(name);
	_line.append("]");
	_line.append(" ");

	// Level
	string level;
	switch (level_)
	{
	case always:
		level = "<Always>    : ";
		break;
	case basic:
		level = "<Basic>     : ";
		break;
	case info:
		level = "<Info>      : ";
		break;
	case debug:
		level = "<Debugging> : ";
		break;
	case success:
		level = "<Success>   : ";
		break;
	case warning:
		level = "<Warning>   : ";
		break;
	case error:
		level = "<Error>     : ";
		break;
	case critical:
		level = "<Critical>  : ";
	}
	_line.append(level);

	// Actual line
	_line.append(line_);
	_line.append("\n");

	// Adding the line to the queue
	log_lines.push(_line);
}


logging logger;