#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>

class FReqs
{
		static std::string *urlStream;
	public:
		std::string URLGet(char *input);
		std::string URLPost(char *input);
	private:
		static int writer(char *data, size_t size, size_t nmemb, std::string *buffer_in);
};

std::string FReqs::URLGet(char *input)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers=NULL;

	curl = curl_easy_init();

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, input);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
		{
			char *ct;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
			if ((CURLE_OK == res) && ct)
				return *urlStream;
		};
	};
};

std::string FReqs::URLPost(char *input)
{
};

int FReqs::writer(char *data, size_t size, size_t nmemb, std::string *buffer_in)
{
	if (buffer_in != NULL)
	{
		buffer_in->append(data, size*nmemb);
		urlStream = buffer_in;
		return size*nmemb;
	}

	return 0;
};

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

/*char* splitChar(char input, char delimeter) {
	std::cout << input << "," << delimeter;
	return input;
};*/

class FHandle
{
	public:
		void callExtension(char *output, int outputSize, const char *function);
};

void FHandle::callExtension(char *output, int outputSize, const char *function)
{	
	strncpy(output, function, outputSize);
};

FHandle *fhandle;

#ifdef __GNUC__

	extern "C"
	{
		void RVExtension(char *output, int outputSize, const char *function);
	};

	void RVExtension(char *output, int outputSize, const char *function)
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
