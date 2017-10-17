
#include <mutex>
#include <string>
#include <string.h>
#include <vector>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <curl/curl.h>
#include <sstream>
#include <queue>
#include <map>
#include "json.hpp"
#include "common.h"

class FetchURL
{
  public:
    struct FetchResult
    {
        std::string result;
        int status; //can be 0 = not completed yet; 1 = completed; 2 = error method, 3 = error on request
    };
    struct FetchQueueItem
    {
        int key;
        std::map<std::string, std::string> params;
    };

    void QueueWorkerThread();
    void InitializeThreads();
    int CallExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
    int AddRequestToQueue(const char **args, int argsCnt);
    int GetStatus(int key);
    std::string GetResult(int key);
    std::string JsonToArray(std::string json);
    void SetFetchResult(int key, FetchURL::FetchResult fres);
    int AddFetchResult(FetchURL::FetchResult fres);
    bool RemoveFetchResult(int key);
  private:
    const char *cmdUrl = "#url";
    const char *cmdMethod = "#method";
    const char *cmdJSONDec = "#jsonToArray";
    const char *actionGET = "GET";
    const char *actionPUT = "PUT";
    const char *actionPOST = "POST";
    const char *actionPATCH = "PATCH";
    const char *actionDELETE = "DELETE";
    const char *actionTRACE = "TRACE";
    const char *paramSep = "=";

    std::mutex queue_mutex;
    std::queue<FetchURL::FetchQueueItem> queue;
    std::mutex resList_mutex;
    std::map<int, FetchURL::FetchResult> resList;

    bool workerInitialized = false;

    std::map<std::string, std::string> parseRequestArgs(const char **args, int argsCnt);
    int addResultToQueue();
    void fetchResult(FetchURL::FetchQueueItem * qItem);
    std::string jsonToArray_object(nlohmann::json j);
    std::string jsonToArray_array(nlohmann::json j);
};
