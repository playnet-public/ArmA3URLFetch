#!/bin/bash
g++ -shared -fPIC -pthread -m32 -o arma3urlfetch.so fetchURL.cpp main.cpp linux/libcurl.a -std=c++11
