
/*
    Filename:
        common.cpp
    Author:
        Vincent Heins / TheMysteriousVincent
    Description:
        This class holds static functions for common things, like converting an string to an int, e.g...
*/

#include "Common.h"

/*
    Publicity: public
    Return type: void
    Name/Type: strReplace()
    Description: replaces each char in *oldChar with *New
*/
void A3URLCommon::StrReplace(std::string *s, const char *oldArr, const char *New)
{
    std::string chars(oldArr);
    for (const char &c : chars)
    {
        //TODO: create for loop for comparison -> less loops.
        size_t found = s->find(c);
        while (found != std::string::npos)
        {
            if (strcmp(New, "") == 0)
            {
                s->erase(found, 1);
            }
            else
            {
                s->replace(found, 1, New);
            }

            found = s->find(c);
        }
    }
}

/*
    Publicity: public
    Return type: int
    Name/Type: strToInt()
    Description: converts a std::string to int !safely!
*/
int A3URLCommon::StrToInt(std::string s)
{
    int i;
    try
    {
        A3URLCommon::StrReplace(&s, "\"'abcdefghijklmnopqrstuvzxyzABCDEFGHIJKLMNOPQRSTUVWXYZöäüÖÄÜ-_*~+#`´?\\ß}{[]()/&%", "");
        i = std::stoi(s);
    }
    catch (const std::invalid_argument &e)
    {
        i = 0;
    }
    catch (const std::out_of_range &e)
    {
        i = 0;
    }

    return i;
};

//A3URLCommon::StrUnquote unquotes a given string pointer
void A3URLCommon::StrUnqoute(std::string *s)
{
    if (s->size() > 0)
    {
        if (s->compare(s->size() - 1, 1, "\"") == 0)
            s->erase(s->size() - 1);

        if (s->compare(0, 1, "\"") == 0)
            s->erase(0, 1);
    }
};

//A3URLCommon::MergeStringMaps merges two given std::map<std::string, std::string> maps
std::map<std::string, std::string> A3URLCommon::MergeStringMaps(std::map<std::string, std::string> m1, std::map<std::string, std::string> m2)
{
    //m2 -> overwrites -> m1
    for (std::map<std::string, std::string>::iterator it = m2.begin(); it != m2.end(); ++it)
        m1[it->first] = it->second;
    
    return m1;
};

//A3URLCommon::ToArray is the init function for the json array to ArmA 3 array convertion
std::string A3URLCommon::ToArray(std::string jTxt)
{
    nlohmann::json j = nlohmann::json::parse(jTxt);
    std::stringstream res;

    if (j.is_array())
    {
        res << "[\"array\"," << A3URLCommon::toArray_array(j) << "]";
    }
    else if (j.is_object())
    {
        res << "[\"object\"," << A3URLCommon::toArray_object(j) << "]";
    }

    return res.str();
};

//A3URLCommon::toArray_array formats only JSON arrays to an ArmA 3 array
std::string A3URLCommon::toArray_array(nlohmann::json j)
{
    std::stringstream res;

    res << "[";
    for (int i = 0; i < j.size(); i++)
    {
        if (i != 0)
            res << ",";

        if (j[i].is_number() || j[i].is_boolean() || j[i].is_string())
        {
            res << j[i];
        }
        else if (j[i].is_null())
        {
            res << "null";
        }
        else if (j[i].is_object())
        {
            res << "[\"object\"," << A3URLCommon::toArray_object(j[i]) << "]";
        }
        else if (j[i].is_array())
        {
            res << "[\"array\"," << A3URLCommon::toArray_array(j[i]) << "]";
        }
    }
    res << "]";

    return res.str();
};

//A3URLCommon::toArray_object formats a JSON object to an ArmA 3 array
std::string A3URLCommon::toArray_object(nlohmann::json j)
{
    std::stringstream res;

    res << "[";
    for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
    {
        if (it != j.begin())
            res << ",";

        if (it.value().is_number() || it.value().is_boolean() || it.value().is_string())
        {
            res << "[\"" << it.key() << "\"," << it.value() << "]";
        }
        else if (it.value().is_null())
        {
            res << "[\"" << it.key() << ",null]";
        }
        else if (it.value().is_object())
        {
            res << "[\"" << it.key() << "\",[\"object\"," << A3URLCommon::toArray_object(it.value()) << "]]";
        }
        else if (it.value().is_array())
        {
            res << "[\"" << it.key() << "\",[\"array\"," << A3URLCommon::toArray_array(it.value()) << "]]";
        }
    }
    res << "]";

    return res.str();
};
