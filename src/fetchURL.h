
#include <mutex>
#include <string>
#include <string.h>
#include <vector>
#include <thread>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <curl/curl.h>

struct FetchResult
{
    int key;
    std::string result;
    bool finished;
};

class FetchURL
{
public:
    void callExtension (const char * output, int outputSize, const char * function);
    std::mutex results_lock;
    std::vector<FetchResult> results;
    void startGETThread(std::string function);
    void startPOSTThread(std::string function, std::string parameters);
    void fetchResultGET(std::string * function);
    void fetchResultPOST(std::string * function, std::string * parameters);
    int returnStatus(int key);
    std::string returnResult(int key);
};
