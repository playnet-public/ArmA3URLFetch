

/*
	Author(s):
		Vincent Heins
	Description:
		This is the main file!?
		I mean, i know this file is a BIT shitty but who cares...
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

/*std::string FReqs::URLGet(const char * input)
{
	std::cout << input << "\n";
	CURL *curl;
        CURLcode res;
        struct curl_slist *headers=NULL;

        curl = curl_easy_init();
	std::cout << input << "\n";

	std::string str;

        if (curl)
        {
		std::cout << input << "\n";
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_URL, input);
                curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackWriter);
		res = curl_easy_perform(curl);
		std::cout << input << "\n";

                if (res == CURLE_OK)
                {
                        char *ct;
                        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
                        if ((CURLE_OK == res) && ct)
				return str;
                }
        }

        return str;
}*/

//like the example from https://curl.haxx.se/libcurl/c/getinmemory.html
static size_t CallbackWriter(void *contents, size_t size, size_t nmemb, void *buf)
{
	((std::string *)buf)->append((char *)contents, size * nmemb);
	return size * nmemb;
};

std::string fetchGET(/*char *output, const int &outputSize,*/ const char *function)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;

	std::cout << function << "\n";

	curl = curl_easy_init();

	std::string str;

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, function);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackWriter);
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
		{
			char *ct;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
			/*if ((CURLE_OK == res) && ct)
                              	break;*/
		}
	}

	str.resize(10240);
	/*const char * c_str = str.c_str();
	strncpy(output, c_str, str.size());*/
	return str;
};

/*struct FetchResult
{
	int key;
	std::string result;
};*/

/*class FetchResulting
{
  public:
	std::mutex * resMtx;
	std::vector<FetchResult *> results;
};

FetchResulting * fres;*/

/*std::mutex resMtx;
std::vector<FetchResult> results;

void fetchResult(const char * function)
{
	std::cout << "inserting fetch for " << function << "\n";

	FetchResult nRes;
	nRes.result = fetchGET(function);

	resMtx.lock();

	std::cout << results.size() << "\n";
	nRes.key = results.size();
	results.push_back(nRes);
	std::cout << "fetch inserted" << function << "\n";

	resMtx.unlock();
};

void newThread(const char *function)
{
	std::thread fetchRequest(fetchResult, function);
};*/

#define URLFETCH_DEVELOPMENT true
#ifdef URLFETCH_DEVELOPMENT

#include <typeinfo>

FetchURL * fURL;

int main ()
{
	const char * input = "STAT|32|5|2"; //"GET|application/json|http://swapi.co/api/people/1/?format=json";
	const char * test = "http://swapi.co/api/people/1/?format=json";
	const char * test2 = "http://swapi.co/api/people/2/?format=json";

	const char * sep = "|";

	std::vector<std::string> nStr;
	std::string strInput(input);

	size_t index = 0;
	for (size_t i = 0; i < strInput.size(); i++)
	{
		if (strInput[i] == *sep)
		{
			if (i == index)
			{
				std::string tmp("");
				nStr.push_back(tmp);
			}
			else
			{
				std::string tmp(strInput.substr(index, i - index));
				nStr.push_back(tmp);
			}
			index = i + 1;
		}
	}

	if (index < strInput.size())
	{
		std::string tmp(strInput.substr(index, strInput.size() - index));
		nStr.push_back(tmp);
	}

	for (int i = 0; i < nStr.size(); i++)
	{
		std::cout << nStr[i] << "\n";
	}

	//std::thread fR1(fetchResult, test);
	//std::thread fR2(fetchResult, test2);
	//newThread(test2);
	//fR1.join();
	//fR2.join();

	//const char * sep = "|";
	//std::string str(input);

	/*if (str[0] == *sep)
	{
		std::cout << "remove first character" << "\n";
		str.erase(str.begin());
	}

	if (str[str.size()-1] == *sep)
	{
		str.erase(str.end());
	}*/

	/*std::vector<std::string> nStr = splitString(input, sep);
	std::cout << nStr.size() << "\n";
	std::cout << nStr[0] << "\n";
	std::cout << stoi(nStr[1]) << "\n";*/

	return 0;
};

#elif

#ifdef __GNUC__

__attribute__((constructor)) void a3urlfetch_initialization()
{
	std::cout << "Sentence which should be displayed on server whilst initializing!"
			  << "\n";
};

extern "C" {
void RVExtension(char *output, int outputSize, const char *function);
};

void RVExtension(char *output, int outputSize, const char *function)
{
	newThread(function);
};

#elif _MSC_VER

#include <windows.h>
#include <shellapi.h>

bool APIENTRY DllMain(HMODULE hMod, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		//call of dll
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		//stop of dll
	}
	break;
	};

	return true;
};

extern "C" {
_declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

void __stdcall RVExtension(char *output, int outputSize, const char *function)
{
	outputSize = -1;
	fhandle->callExtension(output, outputSize, function);
};

#endif

#endif
