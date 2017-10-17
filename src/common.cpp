
#include "common.h"

/*
File for common used functions
*/

/*
    Publicity: public
    Return type: void
    Name/Type: strReplace()
    Description: replaces each char in *oldChar with *New
*/
void A3URLCommon::strReplace(std::string *s, const char *oldArr, const char *New)
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
int A3URLCommon::strToInt(std::string s)
{
    int i;
    try
    {
        A3URLCommon::strReplace(&s, "\"'abcdefghijklmnopqrstuvzxyzABCDEFGHIJKLMNOPQRSTUVWXYZöäüÖÄÜ-_*~+#`´?\\ß}{[]()/&%", "");
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

void A3URLCommon::strUnqoute(std::string *s)
{
    if (s->size() > 0)
    {
        if (s->compare(s->size() - 1, 1, "\"") == 0)
            s->erase(s->size() - 1);

        if (s->compare(0, 1, "\"") == 0)
            s->erase(0, 1);
    }
};
