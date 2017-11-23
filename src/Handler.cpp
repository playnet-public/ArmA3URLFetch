
#include "Handler.h"
#include <iostream>

Handler::Handler()
{
    clients = new Clients();
    requests = new Requests();
};

int Handler::CallExtensionArgs(char * output, int outputSize, const char *function, const char **args, int argsCnt)
{
    if (argsCnt <= 0)
        return 2;
    
    Output *op = Output::Create();
    int rC = 2;

    if (strcmp(function, "ADDCLI") == 0)
    {
        std::map<std::string, std::string> params = parseArgs(args, argsCnt);
        rC = clients->AddClient(op, params);
    }
    else if (strcmp(function, "REMCLI") == 0)
    {
        int id = A3URLCommon::StrToInt(std::string(args[0]));
        rC = clients->RemoveClient(op, id);
    }
    else if (strcmp(function, "SETCLIP") == 0)
    {
        std::map<std::string, std::string> params = parseArgs(args, argsCnt);
        int id = A3URLCommon::StrToInt(params["#clientid"]);
        params.erase(params.find("#clientid"));
        rC = clients->SetParameters(op, id, params);
    }
    else if (strcmp(function, "SETCLIH") == 0)
    {
        std::map<std::string, std::string> params = parseArgs(args, argsCnt);
        int id = A3URLCommon::StrToInt(params["#clientid"]);
        params.erase(params.find("#clientid"));
        std::vector<std::string> headers;
        for (std::map<std::string, std::string>::iterator it = params.begin(); it != params.end(); ++it)
            headers.push_back(it->first);

        rC = clients->SetHeaders(op, id, headers);
    }
    else if (strcmp(function, "SENDRQ") == 0)
    {
        std::map<std::string, std::string> params = parseArgs(args, argsCnt);
        int cID = A3URLCommon::StrToInt(params["#clientid"]);

        if (cID > 0)
        {
            Clients::Client cli;
            if (clients->GetClient(cID, &cli))
            {
                std::map<std::string, std::string> nParams;
                nParams = A3URLCommon::MergeStringMaps(cli.Parameters, params);
                rC = requests->AddRequest(op, nParams, cli.Headers);
            }
        }
        else
        {
            rC = requests->AddRequest(op, params);
        }
    }
    else if (strcmp(function, "GETRQ") == 0)
    {
        int id = A3URLCommon::StrToInt(std::string(args[0]));
        rC = requests->GetResult(op, id);
    }
    
    op->WriteBufFlush(output, outputSize);
    
    delete op;
    return rC;
};

std::map<std::string, std::string> Handler::parseArgs(const char **args, int argsCnt)
{
    std::map<std::string, std::string> params;
    
    for (int x = 0; x < argsCnt; x++)
    {
        std::string param(args[x]);

        if (param.size() > 0)
        {
            A3URLCommon::StrUnqoute(&param);
            size_t i = param.find("=");

            if (i != std::string::npos)
                params[param.substr(0, i)] = param.substr(i + 1, param.length() - i);
            else
                params[param] = std::string("");
        }
    }

    return params;
};
