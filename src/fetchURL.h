
#include <mutex>
#include <string>

struct FetchResult
{
    int key;
    std::string result;
    bool finished;
};

class FetchURL
{
public:
    void callExtension (const char * output, int outputSize, const char * function);
private:
    std::mutex fResults_lock;
    std::vector<FetchResult> fResults;
    
    void startGETThread(std::string function, std::string parameters);
    void startPOSTThread(std::string function, std::string parameters);

    void fetchResultGET(std::string * function, std::string * parameters);
    void fetchResultPOST(std::string * function, std::string * parameters);

    int returnStatus(int key);
    std::string returnResult(int key);
};
