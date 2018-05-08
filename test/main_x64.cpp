
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
#include "../src/handler.h"
#include <thread>
#include <chrono>

int main() {
    Handler *hndl = new Handler();

    char *output = new char[8192];
    int outputSize = 8192;
    const char *function = "SENDRQ";
    const char *args[3];
    const char *tmp1 = "#url=https://httpbin.org/get";
    const char *tmp2 = "#method=GET";
    const char *tmp3 = "test=iiii";
    args[0] = tmp1;
    args[1] = tmp2;
    args[2] = tmp3;

    std::cout << hndl->CallExtensionArgs(output, outputSize, function, args, 3) << std::endl;

    const char *func1 = "GETRQ";
    const char *tmp4 = "1";
    const char *args1[1];
    args1[0] = tmp4;
    std::cout << output << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << hndl->CallExtensionArgs(output, outputSize, func1, args1, 1) << std::endl;
    std::cout << output << std::endl;

    return 0;
}
