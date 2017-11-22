
#pragma once
#include <map>
#include <vector>
#include <mutex>
#include "Output.h"

class Clients
{
public:
    struct Client
    {
        std::map<std::string, std::string> Parameters;
        std::vector<std::string> Headers;
    };
    int AddClient(Output *op, std::map<std::string, std::string> params);
    int RemoveClient(Output *op, int id);
    int SetParameters(Output *op, int id, std::map<std::string, std::string> params);
    int SetHeaders(Output *op, int id, std::vector<std::string> headers);

    //Clients::GetClient returns a client by a given id
    bool GetClient(int id, Clients::Client *client);
private:
    std::map<int, Clients::Client> clients;
    std::mutex clientsMtx;
    int addClient(std::map<std::string, std::string> params);
    bool removeClient(int id);

    //Clients::setClient sets a client struct by given id and Clients::Client struct
    void setClient(int id, Clients::Client cli);
    bool setParameters(int id, std::map<std::string, std::string> params);
    bool setHeaders(int id, std::vector<std::string> headers);
};
