
/*
    Filename:
        common.cpp
    Author:
        Vincent Heins / TheMysteriousVincent
    Description:
        This class holds static functions for common things, like converting an string to an int, e.g...
*/

#include "common.h"

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
        if (s->front() == '"')
            s->erase(0, 1);
        if (s->back() == '"')
            s->erase(s->size() - 1);
        
        //unquote '""' quotes to '"'
        bool lastQuoted = false;
        for (std::string::iterator it = s->begin(); it != s->end(); it++)
        {
            if (*it == '"')
            {
                if (!lastQuoted)
                {
                    lastQuoted = true;
                }
                else
                {
                    s->erase(it);
                    it--;
                    lastQuoted = false;
                }
            }
            else
            {
                lastQuoted = false;
            }
        }
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
    Json::Value root;
    std::stringstream(jTxt) >> root;
    std::stringstream res;

    if (root.empty() || root.isNull())
    {
        res << "[]";
    }
    else if (root.isArray())
    {
        res << A3URLCommon::toArray_array(root);
    }
    else if (root.isObject())
    {
        res << A3URLCommon::toArray_object(root);
    }

    return res.str();
};

//A3URLCommon::toArray_array formats only JSON arrays to an ArmA 3 array
std::string A3URLCommon::toArray_array(const Json::Value &root)
{
    std::stringstream res;

    res << "[\"array\",[";
    if (root.size() > 0)
    {
        for (Json::Value::const_iterator it = root.begin(); it != root.end(); it++)
        {
            if (it != root.begin())
                res << ",";
            
            if (it->isBool())
            {
                res << Json::valueToString(it->asBool());
            }
            else if (it->isInt())
            {
                res << Json::valueToString(it->asInt());
            }
            else if (it->isInt64())
            {
                res << Json::valueToString(it->asInt64());
            }
            else if (it->isUInt())
            {
                res << Json::valueToString(it->asUInt());
            }
            else if (it->isUInt64())
            {
                res << Json::valueToString(it->asUInt64());
            }
            else if (it->isDouble())
            {
                res << Json::valueToString(it->asDouble());
            }
            else if (it->isString())
            {
                res << Json::valueToQuotedString(it->asString().c_str());
            }
            else if (it->isObject())
            {
                res << A3URLCommon::toArray_object(*it);
            }
            else if (it->isArray())
            {
                res << A3URLCommon::toArray_array(*it);
            }
            else
            {
                res << "objNull";
            }
        }
    }
    res << "]]";

    return res.str();
};

//A3URLCommon::toArray_object formats a JSON object to an ArmA 3 array
std::string A3URLCommon::toArray_object(const Json::Value &root)
{
    std::stringstream res;

    res << "[\"object\",[";
    if (root.size() > 0)
    {
        for (Json::Value::const_iterator it = root.begin(); it != root.end(); it++)
        {
            if (it != root.begin())
                res << ",";
            
            res << "[" << Json::valueToQuotedString(it.key().asString().c_str()) << ",";

            if (it->isBool())
            {
                res << Json::valueToString(it->asBool());
            }
            else if (it->isInt())
            {
                res << Json::valueToString(it->asInt());
            }
            else if (it->isInt64())
            {
                res << Json::valueToString(it->asInt64());
            }
            else if (it->isUInt())
            {
                res << Json::valueToString(it->asUInt());
            }
            else if (it->isUInt64())
            {
                res << Json::valueToString(it->asUInt64());
            }
            else if (it->isDouble())
            {
                res << Json::valueToString(it->asDouble());
            }
            else if (it->isString())
            {
                res << Json::valueToQuotedString(it->asString().c_str());
            }
            else if (it->isObject())
            {
                res << A3URLCommon::toArray_object(*it);
            }
            else if (it->isArray())
            {
                res << A3URLCommon::toArray_array(*it);
            }
            else
            {
                res << "objNull";
            }

            res << "]";
        }
    }

    res << "]]";

    return res.str();
};
