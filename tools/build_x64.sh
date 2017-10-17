#!/bin/bash

cd ../
g++ -shared \
    -fPIC \
    -pthread \
    -o arma3urlfetch_x64.so \
    src/fetchURL.cpp \
    src/json.hpp \
    src/main.cpp \
    src/common.cpp \
    src/lib/linux/libcurl_x64.a \
    src/lib/linux/libssl_x64.a \
    src/lib/linux/libcrypto_x64.a \
    -std=c++11
