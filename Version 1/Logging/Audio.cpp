#include "Audio.h"


// Basics
Audio::Audio(void)
{
	logger.addline("Trying to find analong input devices.", debug);
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
	{
		IEnumMoniker *pEnum;
		logger.addline("Analong input device(s) has been detected.", info);

		hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
		if (SUCCEEDED(hr))
		{
			logger.addline("Have found one or more video input devices", success);
			LogDeviceInformation(pEnum);
			pEnum->Release();
		}
		else { logger.addline("No video input device(s) have been detected.", info); }
		hr = EnumerateDevices(CLSID_AudioInputDeviceCategory, &pEnum);
		logger.addline("Finding out all audio input devices", debug);
		if (SUCCEEDED(hr))
		{
			LogDeviceInformation(pEnum);
			pEnum->Release();
		}
		else { logger.addline("No audio input device(s) have been detected.", info); }
		CoUninitialize();
	}
	else{ logger.addline("Have found no analong input devices", debug); }
	logger.addline("All devices as of the start of the program have been detected.", info);
}
Audio::~Audio(void)
{
}

// Private Functions
string Audio::ConvertBSTRToMBS(BSTR bstr)
{
	int wslen = SysStringLen(bstr);
	return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}

string Audio::ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

	std::string dblstr(len, '\0');
	len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
		pstr, wslen /* not necessary NULL-terminated */,
		&dblstr[0], len,
		NULL, NULL /* no default char */);

	return dblstr;
}

void Audio::LogDeviceInformation(IEnumMoniker *pEnum)
{
	logger.addline("Starting to record audio or video input devices.", info);
	IMoniker *pMoniker = NULL;

	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		logger.addline("Have found a new device", success);
		IPropertyBag *pPropBag;
		HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;
		}

		VARIANT var;
		VariantInit(&var);

		// Get description or friendly name.
		hr = pPropBag->Read(L"Description", &var, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
		}
		if (SUCCEEDED(hr))
		{
			//printf("%S\n", var.bstrVal);
			logger.addline("Below is the device name.", debug);
			logger.addline(ConvertBSTRToMBS(var.bstrVal), debug);
			VariantClear(&var);
		}

		hr = pPropBag->Write(L"FriendlyName", &var);

		// WaveInID applies only to audio capture devices.
		hr = pPropBag->Read(L"WaveInID", &var, 0);
		if (SUCCEEDED(hr))
		{
			//printf("WaveIn ID: %d\n", var.lVal);
			logger.addline("Below is the WaveIn ID.", debug);
			logger.addline("BLANK FOR NOW! FIX ME LATER", debug);
			VariantClear(&var);
		}

		hr = pPropBag->Read(L"DevicePath", &var, 0);
		if (SUCCEEDED(hr))
		{
			// The device path is not intended for display.
			printf("Device path: %S\n", var.bstrVal);
			logger.addline("Below is the device path.", debug);
			//logger.addline(ConvertBSTRToMBS(var.bstrVal), success);
			VariantClear(&var);
		}

		pPropBag->Release();
		pMoniker->Release();
	}
}

HRESULT Audio::EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
{
	// Create the System Device Enumerator.
	ICreateDevEnum *pDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));

	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the category.
		hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
		}
		pDevEnum->Release();
	}
	return hr;
}