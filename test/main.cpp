
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
    const char *args[10];
    const char *tmp1 = "\"#url\"";
    const char *tmp2 = "\"https://httpbin.org/get\"";
    const char *tmp3 = "\"#postData\"";
    const char *tmp4 = "\"test\"";
    const char *tmp5 = "\"#headers\"";
    const char *tmp6 = "\"User-Afhjfjfgent\"";
    const char *tmp7 = "\"testtttt\"";
    args[0] = tmp1;
    args[1] = tmp2;
    args[2] = tmp3;
    args[3] = tmp4;
    args[4] = tmp5;
    args[5] = tmp6;
    args[6] = tmp7;

    hndl->CallExtensionArgs(output, outputSize, function, args, 7);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    const char *args2[1];
    args2[0] = std::string(output).c_str();
    std::cout << args2[0] << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    hndl->CallExtensionArgs(output, outputSize, "GETRQ", args2, 1);
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "Time Results:" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << std::endl << "---" << std::endl;

    std::cout << output << std::endl;
}

int main() {
    int i = 0;
    for (i = 0; i < 100; i++) {
        std::cout << "starting thread " << i << std::endl;
        std::thread t(makeRequest);
        t.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}
