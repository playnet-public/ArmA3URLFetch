
#include "requests.h"
#include "macros.h"
#include <iostream>

#ifdef __linux__
Requests::Requests() {
    startWorkers();
};
#endif

//Requests::getPopRequest returns the first request of a queue
void Requests::getPopRequest(Requests::Request *req)
{
    *req = requestsQueue.front();
    requestsQueue.pop();
};

//Request::workerThread is the main function for the worker thread(s)
void Requests::workerThread()
{
    while (true)
    {
        if (!requestsQueue.empty())
        {
            requestsQueueMtx.lock();
            if (!requestsQueue.empty())
            {
                Requests::Request req;
                getPopRequest(&req);
                fetchRequest(req);
            }
            requestsQueueMtx.unlock();
        }
        else
        {
            std::this_thread::yield();
        }
    }
};

//Requests::startWorkers initialize and setups the used worker threads once a time
void Requests::startWorkers()
{
    if (!workersStarted)
    {
        workersStarted = true;

        for (int i = 0; i < THREADS; i++)
        {
            std::thread newThread(&Requests::workerThread, this);
            newThread.detach();
        };
    }
};

//RequestsCurlCallbackWriter is a function used by cURL to receive the incoming byte packes of the URL content
static size_t RequestsCurlCallbackWriter(void *contents, size_t size, size_t nmemb, void *buf)
{
    ((std::string *)buf)->append((char *)contents, size * nmemb);
    return size * nmemb;
};

//Requests::isValidMethod checks for the existance of a given http method
bool Requests::isValidMethod(std::string method)
{
    if (
        method.compare("GET") == 0 ||
        method.compare("POST") == 0 ||
        method.compare("PUT") == 0 ||
        method.compare("PATCH") == 0 ||
        method.compare("DELETE") == 0 ||
        method.compare("TRACE") == 0
    ) return true;
    return false;
};

//Requests::isValidParameter checks if given parameter is existing or valid
bool Requests::isValidParameter(std::string param)
{
    if (
        param.compare("#url") == 0 ||
        param.compare("#method") == 0 ||
        param.compare("#clientid") == 0 ||
        param.compare("#jsonToArray") == 0
    ) return false;
    return true;
};

//Requests::addResult adds a struct of Requests::Result to the result map and returns its id
int Requests::addResult()
{
    int key = 1;

    resultsMtx.lock_shared();
    while (true)
    {
        if (results.find(key) == results.end())
            break;
        key++;
    };
    resultsMtx.unlock_shared();

    Requests::Result res;
    res.status = 0;
    resultsMtx.lock();
    results.insert(std::pair<int, Requests::Result>(key, res));
    resultsMtx.unlock();

    #ifdef _MSC_VER
    startWorkers();
    #endif

    return key;
};

int Requests::addRequest(Arguments::Parameters params)
{
    int key = addResult();
    
    if (key < 1)
        return 0;

    Requests::Request req;
    req.RequestID = key;
    req.Url = params.Url;
    req.Method = params.Method;
    req.PostData = params.PostData;
    req.Headers = params.Headers;
    req.JsonToArray = params.JsonToArray;

    requestsQueueMtx.lock();
    requestsQueue.push(req);
    requestsQueueMtx.unlock();

    return key;
};

//Requests::setResult sets a specific result by its id
void Requests::setResult(int id, Requests::Result res)
{
    if (id <= 0)
        return;

    resultsMtx.lock();

    if (results.find(id) != results.end())
    {
        results[id] = res;
    }

    resultsMtx.unlock();
};

//Requests::removeResult removes an existing result from the map
bool Requests::removeResult(int id)
{
    std::map<int, Requests::Result>::iterator f;
    f = results.find(id);
    if (f == results.end())
        return false;
    
    resultsMtx.lock();
    results.erase(f);
    resultsMtx.unlock();

    return true;
};

//Requests::getResult sets the address of an given result pointer and return its status
int Requests::getResult(int id, Requests::Result *res)
{
    if (results.find(id) == results.end())
        return 2;
    
    resultsMtx.lock_shared();
    *res = results[id]; //error 3 on request...
    resultsMtx.unlock_shared();

    if (res->status > 0) removeResult(id);

    return res->status;
}; //0, 1, 2, 3

//Requests::fetchRequest processes a given request by the parameters of Requests::Request
void Requests::fetchRequest(Requests::Request req)
{
    if (!results.empty())
    {
        Requests::Result res;

        int status = getResult(req.RequestID, &res);
        
        if (status == 0) {
            if (!isValidMethod(req.Method))
            {
                res.status = 2;
            }
            else
            {
                res.status = 3;

                CURL *curl;
                CURLcode cS;

                curl = curl_easy_init();

                struct curl_slist *headers = NULL;
                for (unsigned int i = 0; i < req.Headers.size(); i++)
                {
                    headers = curl_slist_append(headers, req.Headers[i].c_str());
                }

                req.Url.append(req.Forms);

                std::string resStr;

                if (curl)
                {
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                    curl_easy_setopt(curl, CURLOPT_URL, req.Url.c_str());
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, HTTP_VERSION);
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, req.Method.c_str());
                    if (!req.Url.empty()) {
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.PostData.c_str());
                    }

                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resStr);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RequestsCurlCallbackWriter);

                    cS = curl_easy_perform(curl);
                    
                    if (cS == CURLE_OK)
                    {
                        if (req.JsonToArray)
                        {
                            resStr = A3URLCommon::ToArray(resStr);
                        }

                        res.result = resStr;
                        res.status = 1;
                    }
                }
            }

            setResult(req.RequestID, res);
        }
    }
};

//Requests::AddRequest call Requests::addRequest and writes the output to an pointer class Output
int Requests::AddRequest(Output *op, Arguments::Parameters params)
{
    int id = addRequest(params);
    op->Write(id);

    if (id <= 0)
        return 501;

    return 500;
}

//Requests::getResultString copies the result of an Requests::Result class to an std::string pointer
int Requests::getResultString(int id, std::string *str)
{
    if (results.find(id) == results.end())
        return 1;
    
    Requests::Result res;
    res.status = getResult(id, &res);

    if (res.status == 2 || res.status == 3)
    {
        removeResult(id);
    }
    else if (res.status == 1)
    {
        if (res.result.size() > 10200)
        {
            *str = res.result.substr(0, 10200);
            res.result.erase(res.result.begin(), (res.result.begin() + 10200));
            setResult(id, res);
        }
        else if (res.result.size() <= 10200)
        {
            *str = res.result;
            removeResult(id);
        }
    }

    return res.status;
};

//Requests::GetResult copies the result of an result to an sstream Output
int Requests::GetResult(Output *op, int id)
{
    std::string str("");
    int status = getResultString(id, &str);

    op->Write(str.c_str()); // IF str IS EQUAL TO "", str IS FULLY EMPTIED

    switch (status)
    {
        case 0:
            return 602;
        case 2:
        case 3:
            return 601;
    }

    return 600;
};

int Requests::GetStatus(int id)
{
    if (results.find(id) == results.end())
        return 704;
    Requests::Result res;
    resultsMtx.lock_shared();
    res = results[id]; //error 3 on request...
    resultsMtx.unlock_shared();
    return 700 + res.status;
}
