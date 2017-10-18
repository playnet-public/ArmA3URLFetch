
/*
    Filename:
        test.cpp
    Author:
        Vincent Heins / TheMysteriousVincent
    Description:
        This is just a test 
*/

#include "fetchURL.h"
#include <chrono>

int main() {
    FetchURL *fURL;
    fURL = new FetchURL();
    fURL->InitializeThreads();

    std::string test("{\"test\": [0, false, \"asd\", {\"testN\": 2, \"testS\": \"test\", \"testB\": true}]}");
    
    char *output = new char[8192];
    int outputSize = 8192;

    const char *tmp1 = "#url=https://httpbin.org/get";
    const char *tmp2 = "#method=GET";
    const char *args[2];
    args[0] = tmp1;
    args[1] = tmp2;
    const char *id = "\"1\"sdgliöjsdg%$&/sdssdf\\";
    const char *action = "STAT";

    const char *args2[1];
    args2[0] = id;
    
    std::cout << "---\n---\n";
    std::cout << fURL->AddRequestToQueue(args, 2) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "---\n";
    fURL->CallExtensionArgs(output, outputSize, action, args2, 1);
    std::cout << output << std::endl;
    std::cout << "---\n";
    std::cout << fURL->GetResult(1) << std::endl;
    std::cout << "---\n";
    std::cout << fURL->JsonToArray(test) << std::endl;
    
    while (true) {}

    return 0;
}
