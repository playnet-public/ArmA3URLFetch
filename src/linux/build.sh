#!/bin/bash
g++ -shared -fPIC -m32 -o test.so ../main.cpp libcurl.a
