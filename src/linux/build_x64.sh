#!/bin/bash
g++ -shared -fPIC -pthread -o arma3urlfetch_x64.so fetchURL.cpp main.cpp linux/libcurl_x64.a -std=c++11
