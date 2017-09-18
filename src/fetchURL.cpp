
#include "fetchURL.h"

void FetchURL::callExtension(char *output, int outputSize, const char *function)
{
    //split the shit
    std::vector<std::string> args;
    std::string input_str(function);

    size_t index = 0;
    for (size_t i = 0; i < input_str.size(); i++)
    {
        if (input_str[i] == *req_seperator)
        {
            if (i == index)
            {
                std::string tmp("");
                args.push_back(tmp);
            }
            else
            {
                std::string tmp(input_str.substr(index, i - index));
                args.push_back(tmp);
            }
            index = i + 1;
        }
    }

    if (index < input_str.size())
    {
        std::string tmp(input_str.substr(index, input_str.size() - index));
        args.push_back(tmp);
    }

    if (args.size() <= 1)
    {
        outputSize = 2;
        strncpy(output, "-1", outputSize);
        return;
    }

    if (strcmp(args[0].c_str(), "GET") == 0)
    {
        if (strcmp(args[1].c_str(), "") == 0)
        {
            outputSize = 2;
            strncpy(output, "-1", outputSize);
            return;
        }

        if (strcmp(args[2].c_str(), "") == 0)
        {
            outputSize = 2;
            strncpy(output, "-1", outputSize);
            return;
        }
        
        FetchURL::FetchResult fres;
        int key = addResult();

        std::vector<FetchURL::FetchParameters> params;
        for (int i = 3; i < args.size(); i++)
        {
            FetchURL::FetchParameters param;
            int found = args[i].find("=");
            if (found != std::string::npos)
            {
                param.name = args[i].substr(0, found);
                param.value = args[i].substr((found + 1), args[i].size()-1);
                if (strcmp(param.name.c_str(), "") != 0 && strcmp(param.value.c_str(), "") != 0)
                    params.push_back(param);
            }
        }

        std::stringstream str_stream;
        str_stream << key << '\0';
        strncpy(output, str_stream.str().c_str(), str_stream.str().size());
        
        #ifdef _MSC_VER
        if (!workerInitialized)
        {
            initializeThreads();
            workerInitialized = true;
        }
        #endif
        
        FetchURL::FetchQueueItem qItem;
        qItem.key = key;
        qItem.action = args[0];
        qItem.header = args[1];
        qItem.url = args[2];
        qItem.params = params;

        list_queue_mutex.lock();
        list_queue.push(qItem);
        list_queue_mutex.unlock();
    }
    else if (strcmp(args[0].c_str(), "POST") == 0)
    {
        if (strcmp(args[1].c_str(), "") == 0)
        {
            outputSize = 2;
            strncpy(output, "-1", outputSize);
            return;
        }

        if (strcmp(args[2].c_str(), "") == 0)
        {
            outputSize = 2;
            strncpy(output, "-1", outputSize);
            return;
        }

        FetchURL::FetchResult fres;
        int key = addResult();

        std::vector<FetchURL::FetchParameters> params;

        for (int i = 3; i < args.size(); i++)
        {
            FetchURL::FetchParameters param;
            int found = args[i].find("=");
            if (found != std::string::npos)
            {
                param.name = args[i].substr(0, found);
                param.value = args[i].substr((found + 1), args[i].size()-1);
                if (strcmp(param.name.c_str(), "") != 0 && strcmp(param.value.c_str(), "") != 0)
                    params.push_back(param);
            }
        }

        std::stringstream str_stream;
        str_stream << key << '\0';
        strncpy(output, str_stream.str().c_str(), str_stream.str().size());

        #ifdef _MSC_VER
        if (!workerInitialized)
        {
            initializeThreads();
            workerInitialized = true;
        }
        #endif
        
        FetchURL::FetchQueueItem qItem;
        qItem.key = key;
        qItem.action = args[0];
        qItem.header = args[1];
        qItem.url = args[2];
        qItem.params = params;

        list_queue_mutex.lock();
        list_queue.push(qItem);
        list_queue_mutex.unlock();
    }
    else if (strcmp(args[0].c_str(), "STAT") == 0)
    {
        std::stringstream str_stream;
        str_stream << returnStatus(stoi(args[1])) << '\0';
        strncpy(output, str_stream.str().c_str(), str_stream.str().size());
        return;
    }
    else if (strcmp(args[0].c_str(), "RECV") == 0)
    {
        std::stringstream str_stream;
        str_stream << returnResult(stoi(args[1])) << '\0';
        strncpy(output, str_stream.str().c_str(), str_stream.str().size());
    }
    else
    {
        outputSize = 2;
        strncpy(output, "-1", outputSize);
        return;
    }
};

void FetchURL::initializeThreads()
{
    int cnt_thr = 2; //(std::thread::hardware_concurrency() / 2);
    if (cnt_thr == 0)
        cnt_thr = 1;
    
    for (int i = 0; i < cnt_thr; i++)
    {
        std::thread myThread(&FetchURL::queueThread, this);
        myThread.detach();
    }
};

int FetchURL::addResult()
{
    int key = -1;
    int i = 0;

    res_map_mutex.lock();
    while (key == -1)
    {
        if (res_map.find(i) == res_map.end())
            key = i;
        i++;
    }
    FetchURL::FetchResult fres;
    fres.status = 0;
    res_map.insert(std::pair<int, FetchURL::FetchResult>(key, fres));
    res_map_mutex.unlock();

    return key;
};

void FetchURL::queueThread()
{
    while (true) {
        if (!list_queue.empty())
        {
            list_queue_mutex.lock();
            if (list_queue.empty())
            {
                list_queue_mutex.unlock();
            }
            else
            {
                FetchURL::FetchQueueItem qItem;
                qItem = list_queue.front();
                list_queue.pop();
                list_queue_mutex.unlock();
                
                if (strcmp(qItem.action.c_str(), "GET") == 0)
                {
                    fetchGETResult(&qItem);
                }
                else if (strcmp(qItem.action.c_str(), "POST") == 0)
                {
                    fetchPOSTResult(&qItem);
                }

                std::cout << "output: " << qItem.key << "\n";
            }
        }
    }
};

static size_t FetchURLcallbackWriter(void *contents, size_t size, size_t nmemb, void *buf)
{
    ((std::string *)buf)->append((char *)contents, size * nmemb);
    return size * nmemb;
};

void FetchURL::fetchGETResult(FetchURL::FetchQueueItem * qItem)
{
    FetchURL::FetchResult fres;

    int key = qItem->key;
    std::string url = qItem->url;
    std::string header = qItem->header;
    std::vector<FetchURL::FetchParameters> params = qItem->params;

    if (res_map.empty())
        return;

    fres = res_map[key];

    CURL *curl;
    CURLcode res;

    struct curl_slist *headers = NULL;
    curl = curl_easy_init();

    if (params.size() > 0)
    url.append("?");
    
    for (int i = 0; i < params.size(); i++)
    {
        if (i != 0)
        url.append("&");
        char * encoded_val = curl_easy_escape(curl, params[i].value.c_str(), 0);
        char * encoded_name = curl_easy_escape(curl, params[i].name.c_str(), 0);
        url.append(encoded_name);
        url.append("=");
        url.append(encoded_val);
    }
    
    std::string str;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FetchURLcallbackWriter);
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
    
    res_map_mutex.lock();
    res_map[key] = fres;
    res_map_mutex.unlock();
};

void FetchURL::fetchPOSTResult(FetchURL::FetchQueueItem * qItem)
{
    FetchURL::FetchResult fres;

    int key = qItem->key;
    std::string url = qItem->url;
    std::string header = qItem->header;
    std::vector<FetchURL::FetchParameters> params = qItem->params;

    if (res_map.empty())
        return;
    
    fres = res_map[key];

    CURL *curl;
    CURLcode res;

    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    
    std::string p_params;
    for (int i = 0; i < params.size(); i++)
    {
        if (i != 0)
            p_params.append("&");
        char * encoded_val = curl_easy_escape(curl, params[i].value.c_str(), 0);
        char * encoded_name = curl_easy_escape(curl, params[i].name.c_str(), 0);
        p_params.append(encoded_name);
        p_params.append("=");
        p_params.append(encoded_val);
    }

    std::string str;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, p_params.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FetchURLcallbackWriter);
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

    res_map_mutex.lock();
    res_map[key] = fres;
    res_map_mutex.unlock();
};

int FetchURL::returnStatus(int key)
{
    if (res_map.empty() || res_map.find(key) == res_map.end())
        return 0;
    
    FetchURL::FetchResult fres;
    fres = res_map[key];

    return fres.status;
};

std::string FetchURL::returnResult(int key)
{
    if (res_map.empty()/* || res_map.find(key) == res_map.end()*/)
        return std::string();
    
    FetchURL::FetchResult fres;
    fres = res_map[key];

    res_map_mutex.lock();
    res_map.erase(key);
    res_map_mutex.unlock();

    return fres.result;
};

/*FetchURL::~FetchURL()
{
    thread_active = false;
};

void FetchURL::queueThread()
{
    thread_active = true;
    while (thread_active)
    {
        if (!queue_commands_post_get.empty())
        {
            queue_mutex.lock();
            FetchQueueItem qItem = queue_commands_post_get[0];
            queue_commands_post_get.erase(queue_commands_post_get.begin());
            queue_mutex.unlock();

            if (strcmp(qItem.type.c_str(), "GET") == 0)
            {
                fetchResultGET(qItem.key, qItem.header, qItem.function, qItem.params);
            }
            else
            {
                fetchResultPOST(qItem.key, qItem.header, qItem.function, qItem.params);
            }
        }
    }
};*/
