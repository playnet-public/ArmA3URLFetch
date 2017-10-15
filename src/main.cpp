/*
	Author(s):
		Vincent Heins
	Description:
		This is the main file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <mutex>
#include <thread>
#include <vector>
#include "fetchURL.h"

#define VERSION "0.7.5"

//initialize pointer object for later use
FetchURL * fURL;

#ifdef __GNUC__

/*
##################################

	Compilation of the linux
	version starts here.

##################################
*/

//setup needed threads
__attribute__((constructor)) void a3urlfetch_initialization()
{
	fURL = new FetchURL();
	fURL->InitializeThreads();
};

//export ArmA 3 specific function(s)
extern "C" {
int RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
void RVExtensionVersion(char *output, int outputSize);
};

/*
returns the current version on extension load
*ArmA 3/C exported function.
*/
void RVExtensionVersion(char *output, int outputSize)
{
	strncpy(output, VERSION, outputSize);
};

/*
the main entry point for the extension.
More about how to use the extension: 
*ArmA 3/C exported function.
*/
int RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt)
{
	return fURL->CallExtensionArgs(output, outputSize, function, args, argsCnt);
};

#elif _MSC_VER

/*
##################################

	Compilation of the windows
	version starts here.

##################################
*/

#include <windows.h>
#include <shellapi.h>

//entry point for windows librarys
bool APIENTRY DllMain(HMODULE hMod, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		fURL = new FetchURL();
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		//stop of dll - no function needed currently.
	}
	break;
	};

	return true;
};

/*
same as in the Linux version.
*/
extern "C" {
_declspec(dllexport) void __stdcall RVExtensionVersion(char *output, int outputSize);
_declspec(dllexport) int __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
};

/*
same as in the Linux version.
*/
void __stdcall RVExtensionVersion(char *output, int outputSize)
{
	strncpy(output, VERSION, outputSize);
};

/*
same as in the Linux version.
*/
int __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt)
{
	return fURL->CallExtensionArgs(output, outputSize, function, args, argsCnt);
};

#endif
