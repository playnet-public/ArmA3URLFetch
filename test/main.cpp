
/*
    Filename:
        main.cpp
    Author:
        Vincent Heins / TheMysteriousVincent
    Description:
        Tests the functionality of the first layer for receiving input data.
*/

#include <iostream>
#include <string.h>
#include <map>
#include "handler.h"
#include <thread>
#include <chrono>

int main() {
    Handler *hndl = new Handler();

    char *output = new char[8192];
    int outputSize = 8192;
    const char *function = "SENDRQ";
    const char *args[10];
    const char *tmp1 = "\"#url\"";
    const char *tmp2 = "\"https://httpbin.org/get\"";
    const char *tmp3 = "\"#parameters\"";
    const char *tmp4 = "\"test\"";
    const char *tmp5 = "\"SomeValue\"";
    const char *tmp6 = "\"test2\"";
    const char *tmp7 = "\"test55Value\"";
    const char *tmp8 = "\"#headers\"";
    const char *tmp9 = "\"User-Afhjfjfgent\"";
    const char *tmp10 = "\"testtttt\"";
    args[0] = tmp1;
    args[1] = tmp2;
    args[2] = tmp3;
    args[3] = tmp4;
    args[4] = tmp5;
    args[5] = tmp6;
    args[6] = tmp7;
    args[7] = tmp8;
    args[8] = tmp9;
    args[9] = tmp10;

    /*std::cout << hndl->CallExtensionArgs(output, outputSize, function, args, 10) << std::endl;
    std::cout << output << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    const char *args2[1];
    args2[0] = "\"1\"";

    std::cout << hndl->CallExtensionArgs(output, outputSize, "GETRQ", args2, 1) << std::endl;
    std::cout << output << std::endl;*/

    return 0;
}
