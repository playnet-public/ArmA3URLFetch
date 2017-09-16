
#include "fetchURL.h"

void FetchURL::callExtension(char *output, int outputSize, const char *function)
{
    std::cout << "test" << "\n";
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

        vec_mutex.lock();
        int key = 0;
        int i = 1;
        while (key == 0)
        {
            bool key_avail = true;
            for (int j = 0; j < vec.size(); j++)
            {
                if (vec[j].key == i)
                {
                    key_avail = false;
                    break;
                }
            }
    
            if (key_avail)
            {
                key = i;
            }
            else
            {
                i++;
            }
        }

        FetchURL::FetchResult fres;
        fres.key = key;
        fres.status = 0;
        vec.push_back(fres);

        vec_mutex.unlock();

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

        startGETThread(&key, &args[1], &args[2], &params);
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

        vec_mutex.lock();
        int key = 0;
        int i = 1;
        while (key == 0)
        {
            bool key_avail = true;
            for (int j = 0; j < vec.size(); j++)
            {
                if (vec[j].key == i)
                {
                    key_avail = false;
                    break;
                }
            }
    
            if (key_avail)
            {
                key = i;
            }
            else
            {
                i++;
            }
        }

        FetchURL::FetchResult fres;
        fres.key = key;
        fres.status = 0;
        vec.push_back(fres);
        std::cout << key << "\n";

        vec_mutex.unlock();

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

        startPOSTThread(&key, &args[1], &args[2], &params);
    }
    else if (strcmp(args[0].c_str(), "STAT") == 0)
    {
        int status = returnStatus(stoi(args[1]));
        strncpy(output, std::to_string(status).c_str(), 1);
        return;
    }
    else if (strcmp(args[0].c_str(), "RECV") == 0)
    {
        std::string res(returnResult(stoi(args[1])));
        if (res.size() > 10240)
            res.resize(10240);
        strncpy(output, res.c_str(), res.size());
        return;
    }
    else
    {
        outputSize = 2;
        strncpy(output, "-1", outputSize);
        return;
    }
};

static size_t FetchURLcallbackWriter(void *contents, size_t size, size_t nmemb, void *buf)
{
    ((std::string *)buf)->append((char *)contents, size * nmemb);
    return size * nmemb;
};

void FetchURL::startGETThread(int *key, std::string *header, std::string *function, std::vector<FetchURL::FetchParameters> *parameters)
{
    std::thread fr_thread(&FetchURL::fetchResultGET, this, key, header, function, parameters);
};

void FetchURL::fetchResultGET(int *key, std::string *header, std::string *function, std::vector<FetchURL::FetchParameters> *parameters)
{
    FetchURL::FetchResult fres;

    if (vec.empty())
        return;
    vec_mutex.lock();
    int fres_i = -1;
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i].key == *key)
        {
            fres = vec[i];
            fres_i = i;
            break;
        }
    }

    vec_mutex.unlock();

    if (fres_i == -1)
        return;

    CURL *curl;
    CURLcode res;

    struct curl_slist *headers = NULL;
    curl = curl_easy_init();

    std::vector<FetchURL::FetchParameters>& params_ref = *parameters;

    if (params_ref.size() > 0)
        function->append("?");
    
    for (int i = 0; i < params_ref.size(); i++)
    {
        if (i != 0)
            function->append("&");
        char * encoded_val = curl_easy_escape(curl, params_ref[i].value.c_str(), 0);
        char * encoded_name = curl_easy_escape(curl, params_ref[i].name.c_str(), 0);
        function->append(encoded_name);
        function->append("=");
        function->append(encoded_val);
    }

    std::string str;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, function->c_str());
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

    vec_mutex.lock();
    vec[fres_i] = fres;
    vec_mutex.unlock();
};

void FetchURL::startPOSTThread(int *key, std::string *header, std::string *function, std::vector<FetchURL::FetchParameters> *parameters)
{
    std::thread fr_thread(&FetchURL::fetchResultPOST, this, key, header, function, parameters);
    fr_thread.join();
};

void FetchURL::fetchResultPOST(int *key, std::string *header, std::string *function, std::vector<FetchURL::FetchParameters> *parameters)
{

    FetchURL::FetchResult fres;

    if (vec.empty())
        return;

    vec_mutex.lock();
    int fres_i = -1;
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i].key == *key)
        {
            fres = vec[i];
            fres_i = i;
            break;
        }
    }

    vec_mutex.unlock();

    if (fres_i == -1)
        return;

    CURL *curl;
    CURLcode res;

    struct curl_slist *headers = NULL;
    curl = curl_easy_init();

    std::vector<FetchURL::FetchParameters>& params_ref = *parameters;
    std::string p_params;
    
    for (int i = 0; i < params_ref.size(); i++)
    {
        if (i != 0)
            p_params.append("&");
        char * encoded_val = curl_easy_escape(curl, params_ref[i].value.c_str(), 0);
        char * encoded_name = curl_easy_escape(curl, params_ref[i].name.c_str(), 0);
        p_params.append(encoded_name);
        p_params.append("=");
        p_params.append(encoded_val);
    }

    std::string str;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, function->c_str());
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
                std::cout << str << "\n";
                fres.result = str;
                fres.status = 1;
            }
        }
    }

    vec_mutex.lock();
    vec[fres_i] = fres;
    vec_mutex.unlock();
};

int FetchURL::returnStatus(int key)
{
    std::lock_guard<std::mutex> lock(vec_mutex);

    if (vec.empty())
        return 0;

    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i].key == key)
        {
            if (vec[i].status == 0 || vec[i].status == 1)
            {
                return vec[i].status;
            }
            else if (vec[i].status == 2)
            {
                return 2;
            }
        }
    }

    return 0;
};

std::string FetchURL::returnResult(int key)
{
    std::lock_guard<std::mutex> lock(vec_mutex);

    std::string str("");
    if (vec.empty())
        return str;

    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i].key == key)
        {
            if (vec[i].status == 0 || vec[i].status == 1)
            {
                str = vec[i].result;
                vec.erase(vec.begin() + (i - 1));
                return str;
            }
            else
            {
                return str;
            }
        }
    }

    return str;
};
