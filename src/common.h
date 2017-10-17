
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
