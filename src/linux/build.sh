#!/bin/bash

g++ -shared -fPIC -m32 -o arma3urlfetch.so main.cpp libcurl.a
