#!/bin/bash
g++ -shared -fPIC -pthread -m32 -o arma3urlfetch.so fetchURL.cpp json.hpp main.cpp linux/libcrypto.a linux/libssl.a linux/libcurl.a -std=c++11
