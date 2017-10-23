
/*
    Filename:
        common.h
    Author:
        Vincent Heins / TheMysteriousVincent
    Description:
        This header file holds static functions of common.cpp, like converting an string to an int, e.g...
*/

#pragma once
#include <string>
#include <string.h>
#include <stdexcept>
#include <map>
#include "json.hpp"

class A3URLCommon
{
public:
    static void StrReplace(std::string *s, const char *oldArr, const char *New);
    static int StrToInt(std::string s);
    static void StrUnqoute(std::string *s);
    static std::string ToArray(std::string jTxt);
    static std::map<std::string, std::string> MergeStringMaps(std::map<std::string, std::string> m1, std::map<std::string, std::string> m2);
private:
    static std::string toArray_object(nlohmann::json j);
    static std::string toArray_array(nlohmann::json j);
};
