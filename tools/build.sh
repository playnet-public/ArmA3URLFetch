#!/bin/bash

cd ../
g++ -shared \
    -fPIC \
    -pthread \
    -m32 \
    -o arma3urlfetch.so \
    src/fetchURL.cpp \
    src/json.hpp \
    src/main.cpp \
    src/common.cpp \
    src/lib/linux/libcurl.a \
    src/lib/linux/libssl.a \
    src/lib/linux/libcrypto.a \
    -std=c++11
