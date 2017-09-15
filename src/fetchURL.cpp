
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

int FetchURL::returnStatus(int key) {};
std::string FetchURL::returnResult(int key) {};
