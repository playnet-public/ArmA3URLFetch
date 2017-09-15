
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
    std::lock_guard<std::mutex> lock(this->results_lock);

    std::cout << this->results.empty() << "\n";
    if (this->results.empty())
        return 0;

    for (int i = 0; i < this->results.size(); i++)
    {
        if (this->results[i].key == key)
        {
            if (this->results[i].finished)
                return 1;
            else
                return 0;
        }
    }
};

std::string FetchURL::returnResult(int key)
{};
