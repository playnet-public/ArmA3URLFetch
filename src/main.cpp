
/*
	Author(s):
		Vincent Heins
	Description:
		This is the main file?
		I mean, i know this file is a BIT shitty but who cares...
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>

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
//like the example from https://curl.haxx.se/libcurl/c/getinmemory.html
static size_t CallbackWriter(void * contents, size_t size, size_t nmemb, void *buf)
{
	((std::string *) buf)->append((char *) contents, size * nmemb);
	/*std::cout << "1" << "\n";
	size_t nL = size * nmemb;
	size_t oL = buf->size();

	try
	{
		buf->resize(oL + nL);
	}
	catch (std::bad_alloc &e)
	{
		std::cout << "test" << "\n";
		return 0;
	}

	std::copy((char *) contents, (char *) contents + nL, buf->begin() + oL);*/
	return size * nmemb;
}

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

/*std::string URLPost(char *input)
{
	std::string test("");
	return test;
}*/

/*
Valid inputs:
	0 - Type - GET or POST,
	1 - Http Header - e.g. application/json,
	2 - URL - e.g. http://www.bistudio.com/newsfeed/arma3_news.php?build=main&language=English
	3 - Post Values - e.g. ?test=2&anotherOne=test -> ... n

	Full example:
		POST example:
			"POST|application/json|http://www.bistudio.com/newsfeed/arma3_news.php|build=main|language=English" callExtension "fetcher";
		GET example
			"GET|application/json|http://www.bistudio.com/newsfeed/arma3_news.php?build=main&language=English" callExtension "fetcher";
*/

//FReqs * freqs;

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
