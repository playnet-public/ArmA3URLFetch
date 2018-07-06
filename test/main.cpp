
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
#include <ctime>

Handler *hndl = new Handler();

void makeRequest() {
    char *output = new char[8192];
    int outputSize = 8192;
    const char *function = "SENDRQ";
    const char *args[12];
    const char *tmp1 = "\"#url\"";
    const char *tmp2 = "\"https://httpbin.org/post\"";
    const char *tmp3 = "\"#postData\"";
    const char *tmp4 = "\"test1\"";
    const char *tmp5 = "\"test2\"";
    const char *tmp6 = "\"test3\"";
    const char *tmp7 = "\"test4\"";
    const char *tmp8 = "\"#headers\"";
    const char *tmp9 = "\"User-Afhjfjfgent\"";
    const char *tmp10 = "\"testtttt\"";
    const char *tmp11 = "\"#method\"";
    const char *tmp12 = "\"POST\"";
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
    args[10] = tmp11;
    args[11] = tmp12;

    hndl->CallExtensionArgs(output, outputSize, function, args, 12);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto start = std::chrono::high_resolution_clock::now();
    const char *args2[1];
    args2[0] = std::string(output).c_str();
    std::cout << args2[0] << std::endl;
    hndl->CallExtensionArgs(output, outputSize, "GETRQ", args2, 1);
    args2[0] = std::string(output).c_str();
    std::cout << args2[0] << std::endl;
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "Time Results:" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << std::endl << "---" << std::endl;

    std::cout << output << std::endl;
}

int main() {
    makeRequest();
    return 0;
}
