
#include "Requests.h"

#ifdef __linux__
Requests::Requests() {
    startWorkers();
};
#endif

void Requests::getPopRequest(Requests::Request *req)
{
    *req = requestsQueue.front();
    requestsQueue.pop();
};

void Requests::Request::SetParameters(std::map<std::string, std::string> params)
{
    Parameters = params;
};

void Requests::Request::SetHeaders(std::vector<std::string> headers)
{
    Headers = headers;
};

void Requests::Result::SetStatus(int n)
{
    status = n;
};

void Requests::Result::SetResult(std::string r)
{
    result = r;
};

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

void Requests::startWorkers()
{
    if (!workersStarted)
    {
        workersStarted = true;
        int ct = 2;

        for (int i = 0; i < ct; i++)
        {
            std::thread newThread(&Requests::workerThread, this);
            newThread.detach();
        };
    }
};

static size_t RequestsCurlCallbackWriter(void *contents, size_t size, size_t nmemb, void *buf)
{
    ((std::string *)buf)->append((char *)contents, size * nmemb);
    return size * nmemb;
};

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

int Requests::addResult()
{
    int key = 1;

    resultsMtx.lock();
    while (true)
    {
        if (results.find(key) == results.end())
            break;
        key++;
    };

    Requests::Result res;
    res.status = 0;
    results.insert(std::pair<int, Requests::Result>(key, res));
    resultsMtx.unlock();

    #ifdef _MSC_VER
    startWorkers();
    #endif

    return key;
};

int Requests::addRequest(std::map<std::string, std::string> params)
{
    int key = addResult();

    if (key < 1)
        return 0;
    
    Requests::Request req;
    req.RequestID = key;
    req.Parameters = params;

    requestsQueueMtx.lock();
    requestsQueue.push(req);
    requestsQueueMtx.unlock();

    return key;
};

int Requests::addRequest(std::vector<std::string> headers)
{
    int key = addResult();

    if (key < 1)
        return 0;
    
    Requests::Request req;
    req.RequestID = key;
    req.Headers = headers;

    requestsQueueMtx.lock();
    requestsQueue.push(req);
    requestsQueueMtx.unlock();
    
    return key;
};

int Requests::addRequest(std::map<std::string, std::string> params, std::vector<std::string> headers)
{
    int key = addResult();

    if (key < 1)
        return 0;

    Requests::Request req;
    req.RequestID = key;
    req.Parameters = params;
    req.Headers = headers;

    requestsQueueMtx.lock();
    requestsQueue.push(req);
    requestsQueueMtx.unlock();
    
    return key;
};

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

int Requests::getResult(int id, Requests::Result *req)
{
    if (results.find(id) == results.end())
        return 2;
    
    resultsMtx.lock();
    *req = results[id]; //error 3 on request...
    resultsMtx.unlock();

    if (req->status > 0)
        removeResult(id);

    return req->status;
}; //0, 1, 2, 3

void Requests::fetchRequest(Requests::Request req)
{
    if (!results.empty())
    {
        Requests::Result res;
        
        if (getResult(req.RequestID, &res) == 0) {
            if (!isValidMethod(req.Parameters["#method"]))
            {
                res.status = 2;
            }
            else
            {
                std::string url = req.Parameters["#url"];

                res.status = 3;

                CURL *curl;
                CURLcode cS;

                curl = curl_easy_init();

                std::string params("");
                int nPCnt = 0;
                bool fB = false;
                for (
                    std::map<std::string, std::string>::iterator it = req.Parameters.begin();
                    it != req.Parameters.end();
                    ++it
                )
                {
                    if (isValidParameter(it->first))
                    {
                        if (fB)
                            params.append("&");
                        else
                            fB = true;
                        
                        params.append(it->first);

                        if (it->first.compare("") != 0)
                        {
                            params.append("=");
                            params.append(it->second);
                        }
                        nPCnt++;
                    }
                }

                if (nPCnt > 0)
                {
                    url.append("?");
                    url.append(params);
                }

                struct curl_slist *headers = NULL;
                for (int i = 0; i < req.Headers.size(); i++)
                {
                    headers = curl_slist_append(headers, req.Headers[i].c_str());
                }

                std::string resStr;

                if (curl)
                {
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, req.Parameters["#method"].c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resStr);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RequestsCurlCallbackWriter);
                    cS = curl_easy_perform(curl);
                    
                    if (cS == CURLE_OK)
                    {                        
                        if (req.Parameters["#jsonToArray"].compare("true") == 0)
                        {
                            resStr = A3URLCommon::ToArray(resStr);
                        }

                        if (resStr.size() > 10240)
                            resStr.resize(10240);
                        
                        res.result = resStr;
                        res.status = 1;
                    }
                }
            }

            setResult(req.RequestID, res);
        }
    }
};

int Requests::AddRequest(Output *op, std::map<std::string, std::string> params)
{
    int id = addRequest(params);
    op->Write(id);

    if (id == 0)
        return 501;

    return 500;
};

int Requests::AddRequest(Output *op, std::vector<std::string> headers)
{
    int id = addRequest(headers);
    op->Write(id);

    if (id == 0)
        return 501;

    return 500;
};

int Requests::AddRequest(Output *op, std::map<std::string, std::string> params, std::vector<std::string> headers)
{
    int id = addRequest(params, headers);
    op->Write(id);

    if (id == 0)
        return 501;
    
    return 500;
};

int Requests::GetResult(Output *op, int id)
{
    Requests::Result res;
    int status = getResult(id, &res);

    op->Write(res.result.c_str());

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
