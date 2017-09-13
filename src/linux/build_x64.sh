#!/bin/bash
g++ -shared -fPIC -pthread -o arma3urlfetch_x64.so ../main.cpp libcurl_x64.a
