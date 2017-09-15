
#include "fetchURL.h"

void FetchURL::callExtension(const char * output, int outputSize, const char * function)
{};
void FetchURL::startGETThread(std::string function)
{};
void FetchURL::startPOSTThread(std::string function, std::string parameters)
{};
void FetchURL::fetchResultGET(std::string * function)
{};
void FetchURL::fetchResultPOST(std::string * function, std::string * parameters)
{};

int FetchURL::returnStatus(int key)
{
    std::vector<FetchResult> res = fResults;

    for (int i = 0; i < res.size(); i++) {
        if (res[i].key == key)
        {
            if (res[i].finished)
                return 1;
            else
                return 0;
        }
    };
};
std::string FetchURL::returnResult(int key)
{};
