
#include "handler.h"
#include <iostream>
#include <array>

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
        rC = this->addClient(op, args, argsCnt);
    }
    else if (strcmp(function, "REMCLI") == 0)
    {
        rC = this->removeClient(op, args, argsCnt);
    }
    else if (strcmp(function, "SETCLI") == 0)
    {
        rC = this->setClient(op, args, argsCnt);
    }
    else if (strcmp(function, "SENDRQ") == 0)
    {
        rC = this->sendRequest(op, args, argsCnt);
    }
    else if (strcmp(function, "GETRQ") == 0)
    {
        rC = this->getRequest(op, args, argsCnt);
    }
    else if (strcmp(function, "GETST") == 0)
    {
        rC = this->getStatus(op, args, argsCnt);
    }

    op->WriteBufFlush(output, outputSize);
    
    delete op;
    return rC;
};

int Handler::addClient(Output *op, const char **args, int argsCnt)
{
    Arguments::Parameters params;
    params.Method = "GET";
    int err = Arguments::ParseArguments(&params, args, argsCnt);
    if (err > 0)
        return err;
    return this->clients->AddClient(op, params);
};

int Handler::getStatus(Output *op, const char **args, int argsCnt)
{
    return requests->GetStatus(A3URLCommon::StrToInt(std::string(args[0])));
};

int Handler::getRequest(Output *op, const char **args, int argsCnt)
{
    return requests->GetResult(op, A3URLCommon::StrToInt(std::string(args[0])));
};

int Handler::sendRequest(Output *op, const char **args, int argsCnt)
{
    Arguments::Parameters params;
    params.Method = "GET";
    params.JsonToArray = false;
    int err = Arguments::ParseArguments(&params, args, argsCnt);
    if (err > 0) return err;
    
    if (params.ClientID > 0) {
        Clients::Client cli;
        if (clients->GetClient(params.ClientID, &cli)) {
            params.Url = cli.Url;
            params.Method = cli.Method;
            params.PostData = cli.PostData;
            params.Headers = cli.Headers;
            params.JsonToArray = cli.JsonToArray;
        }
    }

    op->Write(params.PostData.c_str());

    return requests->AddRequest(op, params);
};

int Handler::removeClient(Output *op, const char **args, int argsCnt)
{
    return this->clients->RemoveClient(op, A3URLCommon::StrToInt(std::string(args[0])));
};

int Handler::setClient(Output *op, const char **args, int argsCnt)
{
    Arguments::Parameters params;
    int err = Arguments::ParseArguments(&params, args, argsCnt);
    if (err > 0)
        return err;
    return this->clients->SetClient(op, params.ClientID, params);
};
