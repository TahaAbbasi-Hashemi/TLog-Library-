#include "pch.h"
#include "TLogger.h"

thread_names g_thread_names;

// update fixes
wstring s2ws(const std::string& s)
{
	// converts a string to wstring.
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

// Thread names
// #########################################################################
thread_names::thread_names()
{
}
thread_names::~thread_names()
{
}
void thread_names::name_the_thread(string name)
{
	// This function pairs the id of the thread to a actual name
	names.insert(make_pair(this_thread::get_id(), name));
}
string thread_names::get_thread_name(void)
{
	// This function returns the name of the thread.
	return names[this_thread::get_id()];
}

// Logger
// ##########################################################################
logging::logging()
{
}
logging::~logging()
{
}
string logging::int_2_str(long long number)
{
	// This turns an integer into a string.
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

	// Creating the main logging directory
	g_thread_names.name_the_thread(thread_name);
	CreateDirectory(s2ws(directory_location).c_str(), NULL);	// this was fixed s2ws function

	// Values
	int degree;
	bool start_reading;
	string date;
	string thread;
	string file;
	string number;
	string message;
	string severity;
	string reading;
	string redone_line;
	string line;
	vector<string> order;

	// Thread loop
	while (alive || !log_queue.empty())
	{
		// Data values
		if (log_queue.empty())// { continue; }	// If this code is properly runnning, this should never happen.
		{
			this_thread::sleep_for(chrono::milliseconds(1));
			continue;
		}	// If there is nothing in the queue, just kindof chill for a while.
		degree = 0;
		start_reading = false;
		date = "";
		thread = "";
		file = "";
		number = "";
		message = "";
		severity = "";
		reading = "None";
		redone_line = "";
		line = "test";
		line = log_queue.front();
		log_queue.pop();
		//line = return_decompresed_string(log_queue_2.front() );
		order = find_order();

		// Organizing Data from the queue
		// Organizing the variables
		for (unsigned int index = 0; index < line.size(); index++)
		{
			// Finding the order of variables

			// Finding the variables
			char character = line[index];
			if (character != between_variable)
			{
				redone_line += character;
			}
			else
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
			if (reading == "None")
			{
				continue;	// This is unformal but makes it much faster. Speed > Formality in cases like this.
			}
			if (reading == "Date") { date += character; }
			else if (reading == "Thread") { thread += character; }
			else if (reading == "File Name") { file += character; }
			else if (reading == "Line Number") { number += character; }
			else if (reading == "Severity") { severity += character; }
			else { message += character; }
		}	// organization

		// Correcting names.
		{	// This has its own section for better readability. 
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

		// Creating Directories
		string date_folder = directory_location + '/' + __DATE__;
		CreateDirectory(s2ws(date_folder).c_str(), NULL);	// was fixed using s2ws function
		vector<string> locations = { "Thread", "File", "Severity", "Everything" };
		for (auto const& location : locations)
		{
			string direct = date_folder + '/' + location;
			CreateDirectory(s2ws(direct).c_str(), NULL);	// was fixed using s2ws function
		}
		/*
		cout << date << " " << previous_date << endl;
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
		}*/

		// Creating Files.
		vector<string> variables = { thread, file, severity, "Everything" };
		int round = 0;
		for (auto&& variable : variables)
		{
			if (variable == "") { round++; continue; }
			// Testing to see if the variable has a document set to it's name.
			auto existant = false;
			for (unsigned int index = 0; index < file_keys.size(); index++)
			{
				if (file_keys[index] == variable)
				{
					existant = true;
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
				upper_file = locations[round];
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
				}	// inner if
			}	// name test if 

		}	// writing to file loop
	}

	// Closing any files that are open
	if (file_keys.size() != 0)
	{
		file_keys.clear();
		for (unsigned int index = 0; index < file_values.size(); index++)
		{
			file_values[index].close();
		}
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
	int number = -1;
	unsigned int index = 0;
	while (number == -1)
	{
		// This was recoded in version 1.4 for speed and code readabilty. 
		if (file_location[index] == '\\')
		{
			number = index;
		}
		index++;
	}
	file_location.erase(number, file_location.size());
	reverse(file_location.begin(), file_location.end());
	return file_location;
}
void logging::init(void)
{
	// This starts up the program.
	// This allows you to constantly close and reopen the logger. 
	// but it is recommended it only happens once.
	if (occured)
	{
		return;
	}
	occured = true;
	previous_date = "None";
	get_shortened_string();
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
	for (unsigned int index = 0; index < line.size(); index++)
	{
		if (line[index] == between_variable)
		{
			line = "This is unable to be logged, because it is using the between variable.\n";
			line += "Please find a way to rewrite the log without the character :";
			line += between_variable;
		}
	}
	string user_line = "";
	// adding tabs to the line
	line = "\t\t" + line;

	string date = get_date_and_time();
	file_name = file_name_only(file_name);
	string line_number_ = int_2_str(line_number);
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
		severity = "Debug";
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
	// append the variabels into the right places.
	for (unsigned int index = 0; index < log_format.size(); index++)
	{
		char current_location = log_format[index];
		if (current_location != '%')
		{
			// This only occurs if we are not dealing with a variable
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
				break;
			default:
				break;
			}
			user_line += between_variable;
			index++;
		}
	}
	vector<char> test = return_compressed_string(user_line);
	log_queue_2.push(return_compressed_string(user_line));	// compressed string storage
	log_queue.push(user_line);								// non compressed string storage
}
vector<string> logging::find_order(void)
{
	/*
	* Returns the order that is currently being used by the logging libary.
	*/
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
bool logging::get_shortened_string()
{
	/*
	* The purpose of this function is to fill the vector called shortended_strings
	* This function reads from a file, and copies each line down.
	* Each line represents character(s).
	* This just stores the lines into a vector doesn't do anything else.
	* Class variables used: Shortended_string, maxium_compressed_string_length
	*/
	ifstream Combined_Characters("Shortended.txt");
	if (!Combined_Characters)
	{
		shortened_strings = {
		"~Null~",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"Q",
		"W",
		"E",
		"R",
		"T",
		"Y",
		"U",
		"I",
		"O",
		"P",
		"A",
		"S",
		"D",
		"F",
		"G",
		"H",
		"J",
		"K",
		"L",
		"Z",
		"X",
		"C",
		"V",
		"B",
		"N",
		"M",
		"q",
		"w",
		"e",
		"r",
		"t",
		"y",
		"u",
		"i",
		"o",
		"p",
		"a",
		"s",
		"d",
		"f",
		"g",
		"h",
		"j",
		"k",
		"l",
		"z",
		"x",
		"c",
		"v",
		"b",
		"n",
		"m",
		"~",
		"`",
		"!",
		"@",
		"#",
		"$",
		"%",
		"^",
		"&",
		"*",
		"(",
		")",
		"_",
		"-",
		"+",
		"=",
		"{",
		"}",
		"[",
		"]",
		"|",
		"\\",
		":",
		";",
		"'",
		"<",
		">",
		",",
		".",
		"?",
		"/",
		"œ",
		" "
		};
		return false;
	}
	string current_line;
	while (getline(Combined_Characters, current_line, '\n'))
	{
		// we can find the longest compressed string length
		if (current_line != null_string)
		{
			if (maxium_compressed_string_length < current_line.length())
			{
				maxium_compressed_string_length = current_line.length();
			}
		}
		shortened_strings.push_back(current_line);
	}
	return true;
}
vector<char> logging::return_compressed_string(string home_string)
{
	/*
	* The purpose of this function is to take a string and compress it.
	* The function returns a vector of characters, because each character is a number
	* This number represents the character or characters it represents.
	* Class variables used: Shortended_string, string_max_size
	*
	* Home string is our reference string. It shouldn't change unless we are adding
	* null characters at the end.
	* Editing string is the string that will constantly be edited, and have characters
	* removed and changed.
	* This is just so that it makes it easier for variable control.
	*
	* There have been questions about why the null_characters are there
	* at the end of the home_string so here is the aligrythum.
	*
	* Let us say our string_max_size is 4, just for my ease.
	* It will check 4 characters at a time.
	* If our home_variable is the word 'Monkey'. We would now add 4 null characters
	* at the end of our home_string. Let our null_string be = '~'
	* Our home_string would now be 'Monkey~~~~'. Now it is going to be chuncked into smaller
	* strings by using editing_string. This will just be shown as [] on our string.
	* editing_string would be 'Monk' but we will show it as '[Monk]ey~~~~' just so you can see why
	* Next we see if we can reduce each editing string. It would then shrink, if it can't fit the set of characters.
	* If we can't store 'Monk' we try 'Mon' and then try 'Mo' then 'M' till it can store it.
	* then we just move down the string 'M[onke]y~~~~' see it just keeps going.
	* lets move further down.
	* 'Mon[key~]~~~' without the null characters we would have an error here. because we are trying to
	* have 4 characters but we would only have 3. This would give us a error, why we have to add the null characters.
	* It would stop when it reaches 'Monkey[~~~~]'.
	*/
	/*
	string editing_string;
	vector<char> new_string;
	// Adding the null characters at the end of the home_string
	for (unsigned int x = 0; x < maxium_compressed_string_length; x++)
	{
		home_string += null_string;
	}

	for (unsigned int x = 0; x < home_string.length() - maxium_compressed_string_length; x++)
	{
		//
		// * Making a smaller string that I can work with.
		// * This makes the string into a set length.
		//
		editing_string = home_string;
		editing_string.erase(x + maxium_compressed_string_length, editing_string.length());
		editing_string.erase(0, x);

		// Checking to see if if I can compress said string
		bool compressed = false;
		for (unsigned int y = 0; y < maxium_compressed_string_length; y++)
		{
			for (unsigned int z = 0; z < shortened_strings.size(); z++)
			{
				if (compressed) { break; }	// cuts this off 
				if (editing_string == shortened_strings[z])
				{
					// We store numbers as characters. The numbers would turn into a character.
					// To decompress we would convert them back into numbers.
					// this should be able to compess anything as long as its a string.
					new_string.push_back(z);
					compressed = true;
					break;
				}
			}
			if (compressed)
			{
				// skipping the characters weve compressed
				x += editing_string.length();
				break;
			}
			editing_string.erase(editing_string.length() - 1, editing_string.length());
		}
		x += editing_string.length();
	}
	return new_string;
	*/
	string editing_string;
	vector<char> new_string;
	// Adding the null characters at the end of the home_string
	for (unsigned int x = 0; x < maxium_compressed_string_length; x++) { home_string += null_string; }

	for (unsigned int x = 0; x < home_string.length() - maxium_compressed_string_length; x++)
	{
		// Making a smaller string that I can work with.
		editing_string = home_string;
		editing_string.erase(x + maxium_compressed_string_length, editing_string.length());
		editing_string.erase(0, x);

		// Checking to see if if I can compress said string
		int compressed_characters = 0;
		for (unsigned int y = 0; y < maxium_compressed_string_length; y++)
		{
			for (unsigned int z = 0; z < shortened_strings.size(); z++)
			{
				if (editing_string == shortened_strings[z])
				{
					// We store numbers as characters. The numbers would turn into a character.
					// To decompress we would convert them back into numbers.
					new_string.push_back(z);
					//cout << "pushed" << endl;
					compressed_characters += editing_string.length() - 1;
					break;
				}
			}
			if (compressed_characters == 0) { editing_string.erase(editing_string.length() - 1, editing_string.length()); }
			else { break; }
		}
		x += compressed_characters;
	}
	return new_string;
}
string logging::return_decompresed_string(vector<char> compressed_string)
{
	/*
	* The purpose of this function is to return a std::string of the compressed file.
	* The way this works is we would have int just moving down the vector and storing groups of characters
	* at a time.
	* Because the conversion between character and int is weird.
	* it doesn't reconize the fact that it is unsigned.
	* to avoid the situation what I had to do was subtract (2^32 - 128)
	* 2^32 is 4 bytes. This problem only occures after the number 127.
	* This little fix just allows me to get to 255 characters
	* anything more and we overload the byte.
	*/
	string return_string = "";
	for (unsigned int x = 0; x < (compressed_string.size()); x++)
	{
		unsigned int given_number = compressed_string[x];
		if (given_number > 255)
		{
			given_number -= 4294967040;
			// subtract 2^32 -128.
		}
		return_string += shortened_strings[given_number];
	}
	return return_string;
}

