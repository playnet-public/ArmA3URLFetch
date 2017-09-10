#!/bin/bash

g++ -shared -fPIC -pthread -o test_x64.so ../main.cpp libcurl_x64.a
