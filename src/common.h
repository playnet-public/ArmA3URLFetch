
/*
    Filename:
        common.h
    Author:
        Vincent Heins / TheMysteriousVincent
    Description:
        This header file holds static functions of common.cpp, like converting an string to an int, e.g...
*/

#include <string>
#include <string.h>
#include <stdexcept>

class A3URLCommon
{
public:
    static void strReplace(std::string *s, const char *oldArr, const char *New);
    static int strToInt(std::string s);
    static void strUnqoute(std::string *s);
};
