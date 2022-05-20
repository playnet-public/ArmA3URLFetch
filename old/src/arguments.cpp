
#include "arguments.h"
#include "common.h"
#include <iostream>
#include <string>

int Arguments::ParseArguments(Arguments::Parameters *params, const char **args, int argsCnt) {
    std::string tmp;
    std::string tmpArg;

    for (int i = 0; i < argsCnt; i++)
    {
        std::string value(args[i]);
        A3URLCommon::StrUnqoute(&value);

        if (value.compare("#url") == 0)
        {
            if (i+1 < argsCnt)
            {
                i++;
                tmp.append(args[i]);
                A3URLCommon::StrUnqoute(&tmp);
                if (tmp.at(0) == '#' || tmp.empty()) return 10;
                params->Url = tmp;
                tmp.clear();
            }
            else
            {
                return 10;
            }
        }
        else if (value.compare("#method") == 0)
        {
            if (i+1 < argsCnt)
            {
                i++;
                tmp.append(args[i]);
                A3URLCommon::StrUnqoute(&tmp);
                if (tmp.at(0) == '#' || tmp.empty()) return 11;
                params->Method = tmp;
                tmp.clear();
            }
            else
            {
                return 11;
            }
        }
        else if (value.compare("#postData") == 0)
        {
            while (i < argsCnt)
            {
                if (i+1 >= argsCnt) break;
                if (strncmp(args[i+1], "\"#", 2) == 0) break;
                tmp.append(args[i+1]);
                A3URLCommon::StrUnqoute(&tmp);
                params->PostData.append(tmp);
                tmp.clear();
                i++;
            }
        }
        else if (value.compare("#headers") == 0)
        {
            while (i < argsCnt)
            {
                if (i+2 >= argsCnt) break;
                if (strncmp(args[i+1], "\"#", 2) == 0 || strncmp(args[i+2], "\"#", 2) == 0) break;
                tmpArg.append(args[i+1]);
                A3URLCommon::StrUnqoute(&tmpArg);
                tmp.append(tmpArg);
                tmpArg.clear();
                tmp.append(": ");
                tmpArg.append(args[i+2]);
                A3URLCommon::StrUnqoute(&tmpArg);
                tmp.append(tmpArg);
                tmpArg.clear();
                params->Headers.push_back(tmp);
                tmp.clear();
                i += 2;
            }
        }
        else if (value.compare("#jsonToArray") == 0)
        {
            params->JsonToArray = true;
        }
        else if (value.compare("#clientid") == 0)
        {
            if (i+1 < argsCnt)
            {
                i++;
                tmp.append(args[i]);
                A3URLCommon::StrUnqoute(&tmp);
                if (tmp.at(0) == '#' || tmp.empty()) return 12;
                params->ClientID = A3URLCommon::StrToInt(tmp);
                tmp.clear();
            }
            else
            {
                return 12;
            }
        }
        else if (value.compare("#redirect") == 0)
        {
            params->Redirect = true;
            if (i+1 < argsCnt) {
                if (strcmp(args[i+1], "#") != 0)
                    continue;
                i++;
                tmp.append(args[i]);
                A3URLCommon::StrUnqoute(&tmp);
                if (tmp.empty()) return 13;
                params->MaxRedirects = A3URLCommon::StrToInt(tmp);
                tmp.clear();
            }
        }
        else if (value.compare("#timeout") == 0)
        {
            if (i+1 < argsCnt)
            {
                i++;
                tmp.append(args[i]);
                A3URLCommon::StrUnqoute(&tmp);
                if (tmp.at(0) == '#' || tmp.empty()) return 14;
                params->MaxTimeout = (long int)(A3URLCommon::StrToInt(tmp));
                tmp.clear();
            }
            else
            {
                return 14;
            }
        }
        tmpArg.clear();
        tmp.clear();
    }

    return 0;
};
