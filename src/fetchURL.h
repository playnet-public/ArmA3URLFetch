
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

class FetchURL
{
  public:
    struct FetchResult
    {
        int key;
        std::string result;
        int status; //can be 0 = not completed yet; 1 = completed; 2 = error
    };
    struct FetchParameters
    {
        std::string name;
        std::string value;
    };
    void callExtension(char *output, int outputSize, const char *function);

  private:
    const char *req_seperator = "|";
    std::mutex vec_mutex;
    std::vector<FetchURL::FetchResult> vec;
    void startGETThread(int *key, std::string *header, std::string *function, std::vector<FetchURL::FetchParameters> *parameters);
    void fetchResultGET(int *key, std::string *header, std::string *function, std::vector<FetchURL::FetchParameters> *parameters);
    void startPOSTThread(int *key, std::string *header, std::string *function, std::vector<FetchURL::FetchParameters> *parameters);
    void fetchResultPOST(int *key, std::string *header, std::string *function, std::vector<FetchURL::FetchParameters> *parameters);
    int returnStatus(int key);
    std::string returnResult(int key);
};
