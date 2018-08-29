
#pragma once
#include <string>
#include <vector>
#include "output.h"

class Arguments {
public:
    struct Parameters {
        int ClientID, MaxRedirects;
        bool JsonToArray, Redirect;
        std::string Url, Method, PostData;
        std::vector<std::string> Headers;
    };

    static int ParseArguments(Parameters *params, const char **args, int argsCnt);
};
