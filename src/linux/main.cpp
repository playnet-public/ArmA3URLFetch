
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

static size_t CallbackWriter(void * contents, size_t size, size_t nmemb, void *buf)
{
	((std::string *) buf)->append((char *) contents, size * nmemb);
	return size * nmemb;
}

class FHandle
{
	public:
		void callExtension(char *output, const int &outputSize, const char *function);
};

void FHandle::callExtension(char *output, const int &outputSize, const char *function)
{
        CURL *curl;
        CURLcode res;
        struct curl_slist *headers=NULL;

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
	const char * c_str = str.c_str();
	strncpy(output, c_str, str.size());
};

FHandle * fhandle;

#ifdef __GNUC__
	extern "C"
	{
		void RVExtension(char * output, int outputSize, const char * function);
	};

	void RVExtension(char * output, int outputSize, const char * function)
	{
		fhandle->callExtension(output, outputSize, function);
	};
#elif _MSC_VER
	extern "C"
	{
		_declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
	};

	void __stdcall RVExtension(char *output, int outputSize, const char *function)
	{
		outputSize = -1;
		fhandle->callExtension(output, outputSize, function);
	};
#endif
