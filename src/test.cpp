
//GCC: g++ -pthread -o m json.hpp fetchURL.cpp test.cpp linux/libcurl_x64.a -lz -std=c++11

#include "fetchURL.h"
#include <chrono>

int main() {
    FetchURL *fURL;
    fURL = new FetchURL();
    fURL->InitializeThreads();

    std::string test("{\"test\": [0, false, \"asd\", {\"testN\": 2, \"testS\": \"test\", \"testB\": true}]}");
    
    const char *output = new char[8192];
    int outputSize = 8192;

    const char *tmp1 = "#url=https://httpbin.org/get";
    const char *tmp2 = "#method=GET";
    const char *args[2];
    args[0] = tmp1;
    args[1] = tmp2;
    const char *action = "RQST";

    std::cout << fURL->AddRequestToQueue(args, 2) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << fURL->GetResult(0) << std::endl;

    std::cout << fURL->JsonToArray(test) << std::endl;
    
    while (true) {}

    return 0;
}
