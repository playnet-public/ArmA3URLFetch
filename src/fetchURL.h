
#include <mutex>
#include <string>
#include <string.h>
#include <vector>
#include <thread>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <curl/curl.h>
#include <sstream>
#include <queue>
#include <map>

/*
input(output, outputSize, function) ->
    "GET" -> addToWorkhorseQueue(QueueItem) - - -> Workhorse() -> mutex_lock() -> pop_front() -> mutex_unlock() -> fetchGETURLResult() -> 
    "POST" -> -> addToWorkhorseQueue(QueueItem) - - -> Workhorse() -> mutex_lock() -> pop_front() -> mutex_unlock() -> fetchPOSTURLResult() -> 
*/

class FetchURL
{
  public:
    struct FetchResult
    {
        std::string result;
        int status; //can be 0 = not completed yet; 1 = completed; 2 = error
    };
    struct FetchParameters
    {
        std::string name;
        std::string value;
    };
    struct FetchQueueItem
    {
        int key;
        std::string action, header, url;
        std::vector<FetchURL::FetchParameters> params;
    };
    void callExtension(char *output, int outputSize, const char *function);
    void initializeThreads();
    void queueThread();
  private:
    const char *req_seperator = "|";
    std::mutex list_queue_mutex;
    std::queue<FetchURL::FetchQueueItem> list_queue;
    #ifdef _MSC_VER
    bool workerInitialized = false;
    #endif

    std::mutex res_map_mutex;
    std::map<int, FetchURL::FetchResult> res_map;

    int addResult();
    void fetchGETResult(FetchURL::FetchQueueItem * qItem);
    void fetchPOSTResult(FetchURL::FetchQueueItem * qItem);
    int returnStatus(int key);
    std::string returnResult(int key);
};
