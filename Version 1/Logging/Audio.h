#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <windows.h>
#include <dshow.h>
#include "logging.h"
#include "thread_names.h"
#pragma comment(lib, "strmiids")	// Not sure what this does...
// But i need it to use audio and video capture devices.

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

// Globals
using namespace std;
extern logging logger;
extern thread_names g_names;

class Audio
{
private:
	// Variable(s)
	// function(s)
	string ConvertBSTRToMBS(BSTR bstr);
	string ConvertWCSToMBS(const wchar_t* pstr, long wslen);
	void LogDeviceInformation(IEnumMoniker *pEnum);
	HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum);
public:
	// Basic Function(s)
	Audio(void);
	~Audio(void);
	// Function(s)
};

