#!/bin/bash
cd ../
g++ -shared -fPIC -pthread -o arma3urlfetch_x64.so fetchURL.cpp json.hpp main.cpp linux/libcrypto.a linux/libssl.a linux/libcurl_x64.a -std=c++11
