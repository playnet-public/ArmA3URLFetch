
#include "fetchURL.h"

/*
    Publicity: private
    Return type: int
    Name/Type: callExtensionArgs()
    Description: default request function.
*/
int FetchURL::CallExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt)
{
    if (argsCnt <= 0)
        return 200;
    
    if (strcmp(function, "RQST") == 0)
    {
        std::stringstream str_stream;
        str_stream << AddRequestToQueue(args, argsCnt) << '\0';
        strncpy(output, str_stream.str().c_str(), outputSize);
        return 200;
    }
    else if (strcmp(function, "STAT") == 0)
    {
        //TODO: Find a more convenient way to convert the int to a string with an ending line.
        std::stringstream str_stream;
        str_stream << GetStatus(atoi(args[0])) << '\0';
        strncpy(output, str_stream.str().c_str(), outputSize);
        return 300;
    }
    else if (strcmp(function, "RECV") == 0)
    {
        //TODO: Find a more convenient way to convert the int to a string with an ending line.
        std::stringstream str_stream;
        str_stream << GetResult(atoi(args[0])) << '\0';
        strncpy(output, str_stream.str().c_str(), outputSize);
        return 400;
    }

    return 0;
};

/*
    Publicity: private
    Return type: std::map<std::string, std::string>
    Name/Type: parseRequestArgs()
    Description: Converts input **args to a map<std::string, std::string>.
*/
std::map<std::string, std::string> FetchURL::parseRequestArgs(const char **args, int argsCnt)
{
    std::map<std::string, std::string> params;

    for (int x = 0; x < argsCnt; x++)
    {
        std::string param(args[x]);

        if (param.size() > 0)
        {
            if (param.compare(param.size() - 1, 1, "\"") == 0)
                param.erase(param.size() - 1);

            if (param.compare(0, 1, "\"") == 0)
                param.erase(0, 1);

            size_t i = param.find(*paramSep);

            if (i != std::string::npos)
            {
                params[param.substr(0, i)] = param.substr(i+1, param.length() - i);
            }
        }
    }

    return params;
};

/*
    Publicity: private
    Return type: int
    Name/Type: addResultToQueue()
    Description: This function adds a result to a special queue for later usage.
*/
int FetchURL::addResultToQueue()
{
    int key = -1;
    int i = 1;

    resList_mutex.lock();
    while (key == -1)
    {
        if (resList.find(i) == resList.end())
            key = i;
        i++;
    }

    FetchURL::FetchResult fres;
    fres.status = 0;
    fres.result = std::string("");
    resList.insert(std::pair<int, FetchURL::FetchResult>(key, fres));
    resList_mutex.unlock();

    return key;
};

/*
    Publicity: public
    Return type: int
    Name/Type: AddRequestToQueue()
    Description: Adds a full request to the queue(s).
*/
int FetchURL::AddRequestToQueue(const char **args, int argsCnt)
{
    /*
    Initialization of Windows worker Threads. Otherwise the .dll crashes.
    */
    #ifdef _MSC_VER
    InitializeThreads();
    #endif

    int key = addResultToQueue();

    FetchURL::FetchQueueItem qItem;
    qItem.key = key;
    qItem.params = parseRequestArgs(args, argsCnt);

    queue_mutex.lock();
    queue.push(qItem);
    queue_mutex.unlock();

    return key;
};

/*
    Publicity: public
    Return type: void
    Name/Type: InitializeThreads()
    Description: Setups worker threads for url fetching.
*/
void FetchURL::InitializeThreads()
{
    if (!workerInitialized)
    {
        workerInitialized = true;
        int cnt_thr = 2; //(std::thread::hardware_concurrency() / 2);
        if (cnt_thr == 0)
            cnt_thr = 1;
        
        for (int i = 0; i < cnt_thr; i++)
        {
            /*
            Create a thread and immediately detach it.
            */
            std::thread myThread(&FetchURL::QueueWorkerThread, this);
            myThread.detach();
        }
    }
};

/*
    Publicity: public
    Return type: void
    Name/Type: QueueWorkerThread()
    Description: Worker thread loop: processes incoming requests asyncroniously to ArmA 3.
*/
void FetchURL::QueueWorkerThread()
{
    while (true) {
        if (!queue.empty())
        {
            queue_mutex.lock();
            if (queue.empty())
            {
                queue_mutex.unlock();
            }
            else
            {
                FetchURL::FetchQueueItem qItem;
                qItem = queue.front();
                queue.pop();
                queue_mutex.unlock();
                
                std::string action = qItem.params["#method"];
                
                if (
                    action.compare(actionGET) == 0 ||
                    action.compare(actionPOST) == 0 ||
                    action.compare(actionPUT) == 0 ||
                    action.compare(actionPATCH) == 0 ||
                    action.compare(actionDELETE) == 0 ||
                    action.compare(actionTRACE) == 0
                )
                {
                    fetchResult(&qItem);
                }
                else
                {
                    resList_mutex.lock();
                    FetchURL::FetchResult fres;
                    fres = resList[qItem.key];
                    fres.status = 2;
                    resList[qItem.key] = fres;
                    resList_mutex.unlock();
                };
            }
        }
    }
};

/*
    Publicity: public/static
    Return type: size_t
    Name/Type: FetchURLCallbackWriter()
    Description: Writes incoming data from curl to a string/buffer.
*/
static size_t FetchURLCallbackWriter(void *contents, size_t size, size_t nmemb, void *buf)
{
    ((std::string *)buf)->append((char *)contents, size * nmemb);
    return size * nmemb;
};

/*
    Publicity: private
    Return type: void
    Name/Type: fetchResult()
    Description: processes a single url request.
*/
void FetchURL::fetchResult(FetchURL::FetchQueueItem * qItem)
{
    FetchURL::FetchResult fres;

    int key = qItem->key;
    std::string url = qItem->params["#url"];
    std::string method = qItem->params["#method"];
    //std::string mime = qItem.params["#mime"]; currently unused

    if (resList.empty())
        return;

    fres = resList[key];
    fres.status = 3;

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if (qItem->params.size() > 0)
        url.append("?");
    
    bool fBind = false;
    for (std::map<std::string, std::string>::iterator iter = qItem->params.begin(); iter != qItem->params.end(); ++iter)
    {
        if (
            iter->first.compare(cmdUrl) != 0 &&
            iter->first.compare(cmdMethod) != 0 &&
            iter->first.compare(cmdJSONDec) != 0
        )
        {
            if (fBind) {
                url.append("&");
            } else {
                fBind = true;
            }

            char * encVal = curl_easy_escape(curl, iter->second.c_str(), 0);
            char * encKey = curl_easy_escape(curl, iter->first.c_str(), 0);
            url.append(encKey);
            url.append("=");
            url.append(encVal);
        }
    }
    
    std::string str;
    struct curl_slist *headers = NULL;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FetchURLCallbackWriter);
        res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            char *ct;
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);

            if ((CURLE_OK == res) && ct)
            {
                if (str.size() > 10240)
                    str.resize(10240);
                
                fres.result = str;
                fres.status = 1;
            }
        }
    }

    if (qItem->params["#jsonToArray"].compare("true") == 0)
    {
        fres.result = JsonToArray(str);
    }

    resList_mutex.lock();
    resList[key] = fres;
    resList_mutex.unlock();
};

/*
    Publicity: public
    Return type: int
    Name/Type: GetStatus()
    Description: returns the status of an result queue item.
*/
int FetchURL::GetStatus(int key)
{
    if (resList.empty() || resList.find(key) == resList.end())
        return 0;
    
    FetchURL::FetchResult fres;
    fres = resList[key];

    return fres.status;
};

/*
    Publicity: public
    Return type: std::string
    Name/Type: GetResult()
    Description: returns the result of an request. If a request finished successful or errored the selected item is removed after the request.
*/
std::string FetchURL::GetResult(int key)
{
    if (resList.empty()/* || resList.find(key) == resList.end()*/)
        return std::string();
    
    FetchURL::FetchResult fres;
    fres = resList[key];
    
    if (fres.status != 0) {
        resList_mutex.lock();
        resList.erase(key);
        resList_mutex.unlock();
    }

    return fres.result;
};

/*
    Publicity: private
    Return type: std::string
    Name/Type: jsonToArray_object()
    Description: subfunction of JsonToArray() and jsonToArray_array() for object iterations.
*/
std::string FetchURL::jsonToArray_object(nlohmann::json j)
{
    std::stringstream res;

    res << "[";
    for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
    {
        if (it != j.begin())
            res << ",";
        
        if (it.value().is_number() || it.value().is_boolean() || it.value().is_string())
        {
            res << "[\"" << it.key() << "\"," << it.value() << "]";
        }
        else if (it.value().is_null())
        {
            res << "[\"" << it.key() << ",null]";
        }
        else if (it.value().is_object())
        {
            res << "[\"" << it.key() << "\",[\"object\"," << jsonToArray_object(it.value()) << "]]";
        }
        else if (it.value().is_array())
        {
            res << "[\"" << it.key() << "\",[\"array\"," << jsonToArray_array(it.value()) << "]]";
        }
    }
    res << "]";

    return res.str();
};

/*
    Publicity: private
    Return type: std::string
    Name/Type: jsonToArray_array()
    Description: subfunction of JsonToArray() and jsonToArray_object() for array iterations.
*/
std::string FetchURL::jsonToArray_array(nlohmann::json j)
{
    std::stringstream res;

    res << "[";
    for (int i = 0; i < j.size(); i++)
    {
        if (i != 0)
            res << ",";
        
        if (j[i].is_number() || j[i].is_boolean() || j[i].is_string())
        {
            res << j[i];
        }
        else if (j[i].is_null())
        {
            res << "null";
        }
        else if (j[i].is_object())
        {
            res << "[\"object\"," << jsonToArray_object(j[i]) << "]";
        }
        else if (j[i].is_array())
        {
            res << "[\"array\"," << jsonToArray_array(j[i]) << "]";
        }
    }
    res << "]";

    return res.str();
};

/*
    Publicity: private
    Return type: std::string
    Name/Type: jsonToArray()
    Description: Parses a JSON input to an ArmA 3 friendly array.
*/
std::string FetchURL::JsonToArray(std::string jsonStr)
{
    nlohmann::json j = nlohmann::json::parse(jsonStr);
    std::stringstream res;

    if (j.is_array()) {
        res << "[\"array\"," << jsonToArray_array(j) << "]";
    }
    else if (j.is_object())
    {
        res << "[\"object\"," << jsonToArray_object(j) << "]";
    }

    return res.str();
};
