
/*!
    \file Common.h
    \brief Common is class for providing general features.
    \author Vincent Heins
    \see https://github.com/playnet-public/ArmA3URLFetch/blob/master/src/Common.cpp
*/

#pragma once
#include <sstream>
#include <string>
#include <string.h>
#include <stdexcept>
#include <map>
#include "nlohmann/json.hpp"
#include "json/json.h"

/*!
    \class A3URLCommon
    \brief A3URLCommon is a static class which contains only static functions.
*/

class A3URLCommon
{
public:
    /*!
        \fn static void StrReplace(std::string *s, const char *oldArr, const char *New)
        \brief Replaces a single chars with New.
        \param s The string to check.
        \param oldArr The char(s) to search for.
        \param New The replacement char(s) for oldArr.
    */
    static void StrReplace(std::string *s, const char *oldArr, const char *New);
    
    /*!
        \fn static int StrToInt(std::string s)
        \brief Formats a string to an int exception safe.
        \param s A string to be converted to an int.
        \return The formatted number. 0 if there was an error.
    */
    static int StrToInt(std::string s);

    /*!
        \fn static void StrUnqoute(std::string *s)
        \brief Removes (outer) ""-quotes from a std::string
        \param s The string to be unquoted.
    */
    static void StrUnqoute(std::string *s);

    /*!
        \fn static std::string ToArray(std::string jTxt)
        \brief Converts an json text to an ArmA 3 compatible array.
        \param jTxt The JSON string to be converted.
        \return The ArmA 3 ready array (as string).
    */
    static std::string ToArray(std::string jTxt);

    /*!
        \fn static std::map<std::string, std::string> MergeStringMaps(std::map<std::string, std::string> m1, std::map<std::string, std::string> m2)
        \brief Merges two maps together.
        \param m1 The map where new entries will be attached.
        \param m2 The map to compare its containments.

        If both maps contain the same index, index value of m1 will be used.
    */
    static std::map<std::string, std::string> MergeStringMaps(std::map<std::string, std::string> m1, std::map<std::string, std::string> m2);
private:
    /*!
        \fn static std::string toArray_object(nlohmann::json j)
        \brief Is a recursive function of A3URLCommon::ToArray().
    */
    static std::string toArray_object(nlohmann::json j);

    /*!
        \fn static std::string toArray_array(nlohmann::json j)
        \brief Is a recursive function of A3URLCommon::ToArray().
    */
    static std::string toArray_array(nlohmann::json j);
};
