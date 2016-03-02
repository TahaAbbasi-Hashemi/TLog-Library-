#include "logging.h"

logging::logging()
{
}
logging::~logging()
{
}
string logging::int_to_string(long long number)
{
	// This turns an intager into a string.
	stringstream ss;
	ss << number;
	return ss.str();
}
void logging::writing_function(void)
{
	//The purpose of this function is to hold a single thread
	//This thread will have the job of simply writing stuff to storage
	//It will also have total and utter control of the logs
	//The only way to access this thread is through the two variables
	//The first one is working.
	//This decides if this function should live or die.
	//The second one is log_lines, this is the queue where all the lines are saved.


	g_thread_names.name_the_thread(thread_name);
	CreateDirectory(directory_location.c_str(), NULL);

	// Thread loop
	while (alive || !log_queue.empty())
	{
		// Data values
		int degree = 0;
		bool start_reading = false;
		string date = "";
		string thread = "";
		string file = "";
		string number = "";
		string message = "";
		string severity = "";
		string reading = "None";
		string redone_line = "";
		string line = log_queue.front();
		vector<string> order = find_order();

		// Organizing Data from the queue
		if (log_queue.empty()) { this_thread::sleep_for(std::chrono::milliseconds(1)); continue; }	// If there is nothing in the queue, just kindof chill for a while.
		else
		{
			// Organizing the variables
			for (unsigned int index = 0; index < line.size(); index++)
			{
				// Finding the order of variables

				// Finding the variables
				char character = line[index];
				if (character == between_variable)
				{
					if (start_reading)
					{
						start_reading = false;
						reading = "None";
						continue;
					}
					start_reading = true;
					reading = order[degree];
					degree++;
				}
				else { redone_line += character; }

				if (reading == "None") { continue; }
				else if (reading == "Date") { date += character; }
				else if (reading == "Thread") { thread += character; }
				else if (reading == "File Name") { file += character; }
				else if (reading == "Line Number") { number += character; }
				else if (reading == "Severity") { severity += character; }
				else { message += character; }
			}
			// Correcting names.
			{
				vector<string> variables = { thread, file, severity, number, message, date };
				for (auto&& variable : variables)
				{
					if (variable != "")
					{
						reverse(variable.begin(), variable.end());
						variable.erase(variable.size() - 1, variable.size());
						reverse(variable.begin(), variable.end());
					}
				}
				thread = variables[0];
				file = variables[1];
				severity = variables[2];
				number = variables[3];
				message = variables[4];
				date = variables[5];
			}
			log_queue.pop();
		}

		// Creating Directories
		string date_folder = directory_location + '/' + __DATE__;
		CreateDirectory(date_folder.c_str(), NULL);
		vector<string> locations = { "Thread", "File", "Severity"};
		{
			for (auto&& location : locations)
			{
				string direct = date_folder + '/' + location;
				CreateDirectory(direct.c_str(), NULL);
			}
		}

		// Cleaning file vectors if I have to.
		if (date != previous_date)
		{
			previous_date = date;
			if (file_keys.size() != 0)
			{
				file_keys.clear();
				for (unsigned int index = 0; index < file_values.size(); index++) { file_values[index].close(); }
				file_values.clear();
			}
		}

		// Creating Files.
		vector<string> variables = { thread, file, severity, "Everything" };
		int round = 0;
		for (auto&& variable : variables)
		{
			if (variable == "") { round++; continue; }
			// Testing to see if the variable has a document set to it's name.
			bool existant = false;
			long existant_index;
			for (unsigned int index = 0; index < file_keys.size(); index++)
			{
				if (file_keys[index] == variable)
				{
					existant = true;
					existant_index = index;
				}
			}
			if (!existant)
			{
				string upper_file;
				if (variable == "Everything")
				{
					string file_location = date_folder + "Everything.log";
					continue;
				}
				else { upper_file = locations[round]; }
				string file_location = date_folder + '/' + upper_file + '/' + variable + ".log";
				ofstream file_stream(file_location, ios::trunc);
				file_keys.push_back(variable);
				file_values.push_back(move(file_stream));
			}
			round++;
		}

		// Writing to Files.
		for (auto&& variable : variables)
		{
			if (variable == "") { continue; }
			// Testing to see if the variable has a document set to it's name.
			for (unsigned int index = 0; index < file_keys.size(); index++)
			{
				if (file_keys[index] == variable)
				{
					file_values[index] << redone_line << '\n';
					break;
				}
			}

		}
	}

	// End Cleaning
	if (file_keys.size() != 0)
	{
		file_keys.clear();
		for (unsigned int index = 0; index < file_values.size(); index++) { file_values[index].close(); }
		file_values.clear();
	}
}
string logging::get_date_and_time(void)
{
	// The purpose of this function is to return the current date
	// along side with the current time.
	string date_time = "'";
	date_time.append(__DATE__);
	date_time.append("' ");

	char buffer[70];		// The variable that will hold the current date and time
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 70, "%X", &timeinfo);

	date_time.append("");
	date_time.append(buffer);
	date_time.append("");
	return date_time;
}
string logging::file_name_only(string file_location)
{
	// The purpose of this function is to edit a variable
	// this variable should be __FILE__
	// one of the builtin Cpp macros.
	// What this function does is it only gives us the name of the file
	// vs giving us the entire directory.
	reverse(file_location.begin(), file_location.end());
	int number;
	for (unsigned int index = 0; index < file_location.size(); index++)
	{
		if (file_location[index] == '\\') { number = index; break; }
	}
	file_location.erase(number, file_location.size());
	reverse(file_location.begin(), file_location.end());
	return file_location;
}
void logging::init(void)
{
	// This starts up the logging system. This can start more then one time
	// but it is recommended it onl happens once.
	if (occured) { return; }
	occured = true;
	writing_thread = thread(&logging::writing_function, this);
}
void logging::kill(void)
{
	// This stops any more logs to be written
	// and then wats for the queue to finish.
	alive = false;
	writing_thread.join();
	occured = false;	// This allows me to run the program again, after killing it.
}
void logging::__addline__(string line, string file_name, int line_number, severity_levels level)
{
	// The purpose of this function is append a line of information into the
	//queue of information.
    //Read the readme.txt to figure out the reasoning behind the code here.
	// checking to make sure we have the between variable in the message
	for (unsigned int index = 0; index<line.size(); index++)
	{
		if (line[index] == between_variable)
		{
			line = "This is unable to be logged, because it is using the between variable.\n";
				line += "Please find a way to rewrite the log without the character :";
				line += between_variable;
		}
	}
	string user_line = "";

	string date = get_date_and_time();
	file_name = file_name_only(file_name);
	string line_number_ = int_to_string(line_number);
	string thread = g_thread_names.get_thread_name();
	string severity;
	switch (level)
	{
	case always:
		severity = "Always";
		break;
	case basic:
		severity = "Basic";
		break;
	case info:
		severity = "Info";
		break;
	case debug:
		severity = "Debugging";
		break;
	case success:
		severity = "Success";
		break;
	case warning:
		severity = "Warning";
		break;
	case error:
		severity = "Error";
		break;
	case critical:
		severity = "Critical";
	}
	// appened the variabels into the right places.
	for (unsigned int index = 0; index<log_format.size(); index++)
	{
		char current_location = log_format[index];
		if (current_location != '%')
		{
			// This only occours if we are not dealing with a variable
			user_line += current_location;
		}
		else
		{
			char variable_character = log_format[index + 1];
			user_line += between_variable;
			switch (variable_character)
			{
			case 'D':
				user_line += date;
				break;
			case 'T':
				user_line += thread;
				break;
			case 'F':
				user_line += file_name;
				break;
			case 'L':
				user_line += line_number_;
				break;
			case 'S':
				user_line += severity;
				break;
			case 'M':
				user_line += line;
			}
			user_line += between_variable;
			index++;
		}
	}
	log_queue.push(user_line);
	//cout << user_line << endl;
}
vector<string> logging::find_order(void)
{
	vector<string> order_appendix;
	for (unsigned int index = 0; index < log_format.size(); index++)
	{
		char current_location = log_format[index];
		if (current_location == '%')
		{
			char variable_character = log_format[index + 1];
			switch (variable_character)
			{
			case 'D':
				order_appendix.push_back("Date");
				break;
			case 'T':
				order_appendix.push_back("Thread");
				break;
			case 'F':
				order_appendix.push_back("File Name");
				break;
			case 'L':
				order_appendix.push_back("Line Number");
				break;
			case 'S':
				order_appendix.push_back("Severity");
				break;
			case 'M':
				order_appendix.push_back("Message");
			}
		}
	}
	return order_appendix;
}

int main()
{
	g_thread_names.name_the_thread("Pretty Decent Thread");

	logging logger;
	logger.addline("Average Log Size. This is all it is, don't worry aboout it.", debug);
	for (long long x = 0; x < 60; x++)
	{
		logger.addline("Unknown", always);
		logger.addline("Unknown", basic);
		logger.addline("Unknown", info);
		logger.addline("Unknown", debug);
		logger.addline("Unknown", success);
		logger.addline("Unknown", warning);
		logger.addline("Unknown", error);
		logger.addline("Unknown", critical);
	}
	logger.init();
	for (long long x = 0; x < 60; x++)
	{
		logger.addline("Unknown", always);
		logger.addline("Unknown", basic);
		logger.addline("Unknown", info);
		logger.addline("Unknown", debug);
		logger.addline("Unknown", success);
		logger.addline("Unknown", warning);
		logger.addline("Unknown", error);
		logger.addline("Unknown", critical);
	}
	logger.kill();
	return 0;
}
