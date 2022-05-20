
#include "macros.h"
#include <stdio.h>
#include "handler.h"

#ifndef __linux__

#include <windows.h>
#include <shellapi.h>

#endif


Handler * hndl;


#ifdef __linux__


__attribute__((constructor)) void a3urlfetch_initialization()
{
	hndl = new Handler();
};


#else


bool APIENTRY DllMain(HMODULE hMod, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		hndl = new Handler();
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


#endif



extern "C"
{
	//Entry point for (I think) gcc compilers
    #ifdef __linux__


    int RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
    void RVExtensionVersion(char *output, int outputSize);

	//Entry point for others (currently just for Windows)
    #else


    _declspec(dllexport) int __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
    _declspec(dllexport) void __stdcall RVExtensionVersion(char *output, int outputSize);


    #endif
};


//Linux exported functions
#ifdef __linux__


void RVExtensionVersion(char *output, int outputSize)
{
	strncpy(output, VERSION, outputSize);
};

int RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt)
{
	return hndl->CallExtensionArgs(output, outputSize, function, args, argsCnt);
};


//Windows exported functions
#else


void __stdcall RVExtensionVersion(char *output, int outputSize)
{
	strncpy(output, VERSION, outputSize);
};

int __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt)
{
	return hndl->CallExtensionArgs(output, outputSize, function, args, argsCnt);
};


#endif


