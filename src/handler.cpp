
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

    op->WriteBufFlush(output, outputSize);
    
    delete op;
    return rC == 0 ? 2 : rC;
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

int Handler::getRequest(Output *op, const char **args, int argsCnt)
{
    std::cout << args[0] << std::endl;
    std::cout << A3URLCommon::StrToInt(std::string(args[0])) << std::endl;
    return requests->GetResult(op, A3URLCommon::StrToInt(std::string(args[0])));
}

int Handler::sendRequest(Output *op, const char **args, int argsCnt)
{
    Arguments::Parameters params;
    params.Method = "GET";
    int err = Arguments::ParseArguments(&params, args, argsCnt);
    if (err > 0) return err;
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
