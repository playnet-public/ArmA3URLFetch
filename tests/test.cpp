
/*
    Filename:
        test.cpp
    Author:
        Vincent Heins / TheMysteriousVincent
    Description:
        This is just a test 
*/

#include <iostream>
#include <string.h>
#include <map>
#include "Handler.h"
#include <thread>
#include <chrono>

int main() {
    Handler *hndl = new Handler();

    char *output = new char[8192];
    int outputSize = 8192;
    const char *function = "addClient";
    const char *args[3];
    const char *tmp1 = "#url=https://httpbin.org/get";
    const char *tmp2 = "#method=GET";
    const char *tmp3 = "p?aram1=param2";
    args[0] = tmp1;
    args[1] = tmp2;
    args[2] = tmp3;

    std::cout << hndl->CallExtensionArgs(output, outputSize, function, args, 3) << std::endl;
    std::cout << output << std::endl;

    char *output_3 = new char[8192];
    int outputSize_3 = 8192;
    const char *function_3 = "setClientParams";
    const char *args_3[2];
    const char *tmp1_3 = "test=2";
    const char *tmp2_3 = "#clientid=1";
    args_3[0] = tmp1_3;
    args_3[1] = tmp2_3;

    std::cout << hndl->CallExtensionArgs(output_3, outputSize_3, function_3, args_3, 2) << std::endl;
    std::cout << output_3 << "\n";

    char *output_1 = new char[8192];
    int outputSize_1 = 8192;
    const char *function_1 = "sendRequest";
    const char *args_1[1];
    const char *tmp1_1 = "#clientid=1";
    args_1[0] = tmp1_1;

    hndl->CallExtensionArgs(output_1, outputSize_1, function_1, args_1, 1);
    std::cout << output_1 << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    char *output_2 = new char[8192];
    int outputSize_2 = 8192;
    const char *function_2 = "getRequestResult";
    const char *args_2[1];
    const char *tmp1_2 = "1";
    args_2[0] = tmp1_2;

    std::cout << hndl->CallExtensionArgs(output_2, outputSize_2, function_2, args_2, 1) << std::endl;
    std::cout << output_2 << std::endl;

    while (true) {}
    return 0;
}
