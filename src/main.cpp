
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

/*struct MemStruct
{
        char * memory;
        size_t size;
};

class FReqs
{
	public:
		std::string URLGet(const char * input);
		//std::string URLPost(char *input);
	private:
		//static size_t writer(void *contents, size_t size, size_t nmemb, void *userp);
		//static int writer(char *data, size_t size, size_t nmemb, std::string *buffer_in);
};
*/

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

struct FetchResult
{
	bool finished;
	int key;
	std::string result;
};

class FetchResulting
{
  public:
	std::mutex resMtx;
	std::vector<FetchResult> results;
};

FetchResulting *fres;

void newThread(const char *function)
{
	std::thread fetchRequest(fetchResult, function);
};

void fetchResult(const char * function)
{
	fres->resMtx->lock();

	FetchResult *nRes;
	nRes.finished = false;
	nRes.key = fres->results->size();
	fres->results->push_back(&nRes);

	fres->resMtx->unlock();
	nRes.result = fetchGET(function);
	nRes.finished = true;
};

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
