#!/bin/bash

cd ../
g++ -shared \
    -fPIC \
    -pthread \
    -m32 \
    -o arma3urlfetch.so \
    src/Common.cpp \
    src/Requests.cpp \
    src/Clients.cpp \
    src/Output.cpp \
    src/Handler.cpp \
    src/json.hpp \
    src/main.cpp \
    lib/linux/libcurl.a \
    lib/linux/libssl.a \
    lib/linux/libcrypto.a \
    -std=c++11
