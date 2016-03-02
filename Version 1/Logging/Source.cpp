#include "logging.h"
#include "thread_names.h"
#include "Point.h"
#include "Segment.h"
#include "Circle.h"
#include "Audio.h"

// Globals
using namespace std;
extern logging logger;
extern thread_names g_names;


int main()
{
	// ########################################
	//				Start up stuff
	// ########################################
	g_names.name_thread("Main Thread");
	// This starts the logging
	logger.addline("A new program has started", always);
	logger.addline("A logging program has just been started", always);
	logger.init();

	// #######################################
	//				General
	// #######################################
	logger.addline("Currently what is being tested is audio", always);
	/*
	These comments are for me.
	Basically, I just want to know what im doing later on in the day

	To get audio from DirectX you have to be using windows.
	Basically it allows me get a analog signal from a soundcard.
	It then turns it into a digital signal using something called a
	"Audio Capture Filter". 
	The thing is if I'm able to get audio, im also able to get video.
	which is very interesting.
	
	I can choose between whats audio inputs I want as well...
	I belive I have to make a emulator for this though.
	Aparently, I have to know exactly where a hardware device is to use it.
	So using a emulator allows me to use said hardware without knowing it's exact position
	Im not familiar with this concept.
	*/
	Audio bob;

	/*
	Basically what I want to do is make a device to do what i want it to do multiple times, 
	each time attempting to imporve on its problems.
	*/
	// #######################################
	//				Ending blocks
	// #######################################
	logger.kill();
	cin.clear(); // reset any error flags
	cin.ignore(32767, '\n'); // ignore any characters in the input buffer until we find an enter character
								//cin.get(); // get one more char from the user
	return 0;
}