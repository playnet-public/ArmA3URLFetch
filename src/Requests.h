
#pragma once
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include "Output.h"
#include "Common.h"

class Requests
{
public:
    #ifdef __linux__
    Requests();
    #endif
    struct Result
    {
        int status;
        std::string result;
        void SetResult(std::string r);
        void SetStatus(int n);
    };
    struct Request
    {
        int RequestID;
        std::map<std::string, std::string> Parameters;
        std::vector<std::string> Headers;
        void SetParameters(std::map<std::string, std::string> params);
        void SetHeaders(std::vector<std::string> headers);
    };

    int AddRequest(Output *op, std::map<std::string, std::string> params);
    int AddRequest(Output *op, std::vector<std::string> headers);
    int AddRequest(Output *op, std::map<std::string, std::string> params, std::vector<std::string> headers);
    int GetResult(Output *op, int id);
private:
    std::map<int, Requests::Result> results;
    std::mutex resultsMtx;
    std::queue<Requests::Request> requestsQueue;
    std::mutex requestsQueueMtx;
    bool workersStarted = false;
    void getPopRequest(Requests::Request *req);
    void startWorkers();
    void fetchRequest(Requests::Request req);
    bool isValidMethod(std::string method);
    bool isValidParameter(std::string param);
    void workerThread();

    int addRequest(std::map<std::string, std::string> params);
    int addRequest(std::vector<std::string> headers);
    int addRequest(std::map<std::string, std::string> params, std::vector<std::string> headers);
    int addResult();
    void setResult(int id, Requests::Result res);
    bool removeResult(int id);
    int getResult(int id, Requests::Result *req); //0, 1, 2
};
