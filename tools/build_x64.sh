#!/bin/bash

cd ../
g++ -shared \
    -fPIC \
    -pthread \
    -o arma3urlfetch_x64.so \
    src/Common.cpp \
    src/Requests.cpp \
    src/Clients.cpp \
    src/Output.cpp \
    src/Handler.cpp \
    src/json.hpp \
    src/main.cpp \
    lib/linux/libcurl_x64.a \
    lib/linux/libssl_x64.a \
    lib/linux/libcrypto_x64.a \
    -std=c++11
