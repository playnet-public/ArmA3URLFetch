
#include "requests.h"
#include "macros.h"
#include <iostream>

#ifdef __linux__
Requests::Requests() {
    startWorkers();
};
#endif

//Request::workerThread is the main function for the worker thread(s)
void Requests::workerThread()
{
    std::unique_lock<std::mutex> lock(m_requests);

    while (1)
    {
        cv_requests.wait(lock, [this]{
            return (requests.size());
        });

        if (requests.size())
        {
            Requests::Request *req = requests.front();
            requests.pop();

            lock.unlock();

            fetchRequest(*req);

            delete req;

            lock.lock();
        }
    }
};

//Requests::startWorkers initialize and setups the used worker threads once a time
void Requests::startWorkers()
{
    if (!workersStarted)
    {
        for (int i = 0; i < THREADS; i++)
        {
            std::thread t(&Requests::workerThread, this);
            t.detach();
        };
        workersStarted = true;
    }
};

//RequestsCurlCallbackWriter is a function used by cURL to receive the incoming byte packes of the URL content
static size_t RequestsCurlCallbackWriter(void *contents, size_t size, size_t nmemb, void *buf)
{
    ((std::string *)buf)->append((char *)contents, size * nmemb);
    return size * nmemb;
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
    int key = lastID++;

    Requests::Result *res = new Requests::Result(1); // 0 = text pending, 1 = pending, 2 = error
    m_results.lock();
    results.insert(std::pair<int, Requests::Result*>(key, res));
    m_results.unlock();

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
    
    std::cout << "key: " << key << std::endl;

    Requests::Request *req = new Requests::Request(
        key,
        params.MaxRedirects,
        params.MaxTimeout,
        params.JsonToArray,
        params.Redirect,
        params.Url,
        params.Method,
        params.PostData,
        params.Headers
    );

    m_requests.lock();
    requests.push(req);
    m_requests.unlock();

    return key;
};

//Requests::removeResult removes an existing result from the map
bool Requests::removeResult(int id)
{
    std::map<int, Requests::Result*>::iterator f;
    f = results.find(id);
    if (f == results.end())
        return false;
    
    m_results.lock();
    results.erase(f);
    delete f->second;
    m_results.unlock();

    return true;
};

//Requests::getResult sets the address of an given result pointer and return its status
Requests::Result* Requests::getResult(int id)
{
    std::lock_guard<std::mutex> lock(m_results);
    //if (res->status == 2) removeResult(id);
    return results[id];
};

//Requests::fetchRequest processes a given request by the parameters of Requests::Request
void Requests::fetchRequest(Requests::Request req)
{
    if (!results.empty())
    {
        Requests::Result *res = getResult(req.RequestID);

        if (res != nullptr) {
            m_results.lock();
            int status = res->status;
            m_results.unlock();

            long httpCode = 0;
            std::string resStr;

            if (status == 1) {
                CURL *curl;
                CURLcode cS;

                curl = curl_easy_init();

                struct curl_slist *headers = NULL;
                for (unsigned int i = 0; i < req.Headers.size(); i++)
                {
                    headers = curl_slist_append(headers, req.Headers[i].c_str());
                }

                if (curl)
                {
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                    curl_easy_setopt(curl, CURLOPT_URL, req.Url.c_str());
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, HTTP_VERSION);
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, req.Method.c_str());

                    if (!req.Url.empty()) {
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.PostData.c_str());
                    }

                    if (req.Redirect) {
                        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                        if (req.MaxRedirects != 0) {
                            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long int)req.MaxRedirects);
                        }
                    }

                    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, req.MaxTimeout);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resStr);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RequestsCurlCallbackWriter);

                    cS = curl_easy_perform(curl);

                    if (cS == CURLE_OK)
                    {
                        if (req.JsonToArray)
                        {
                            resStr = A3URLCommon::ToArray(resStr);
                        }

                        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
                        status = 0;
                    } else {
                        status = 2;
                    }
                } else {
                    status = 2;
                }

                m_results.lock();
                res->status = status;
                res->httpCode = httpCode;
                res->result = resStr;
                m_results.unlock();
            }
        }
    }
};

//Requests::AddRequest call Requests::addRequest and writes the output to an pointer class Output
int Requests::AddRequest(Output *op, Arguments::Parameters params)
{
    int id = addRequest(params);
    cv_requests.notify_one();
    op->Write(id);

    if (id <= 0)
        return 0;

    return 0;
}

//Requests::getResultString copies the result of an Requests::Result class to an std::string pointer
int Requests::getResultString(int id, std::string *str)
{
    if (results.find(id) == results.end())
        return 2;

    Requests::Result res = *getResult(id);

    if (res.status == 0)
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
            res.status = res.httpCode;
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

    return status;
};

int Requests::GetStatus(int id)
{
    std::lock_guard<std::mutex> lock(m_results);

    Requests::Result *res = results[id];
    return (res == nullptr) ? 3 : res->status;
}
