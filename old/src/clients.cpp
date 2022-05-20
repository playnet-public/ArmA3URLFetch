
#include "clients.h"

bool Clients::GetClient(int id, Clients::Client *client)
{
    if (clients.find(id) == clients.end())
        return false;
    
    clientsMtx.lock();
    *client = clients[id];
    clientsMtx.unlock();

    return true;
};

void Clients::setClient(int id, Clients::Client cli)
{
    clientsMtx.lock();
    if (clients.find(id) != clients.end())
        clients[id] = cli;
    clientsMtx.unlock();
};

bool Clients::setClient(int id, Arguments::Parameters params)
{
    Clients::Client client;
    if (!GetClient(id, &client))
        return false;
    
    client.Url = params.Url;
    client.Method = params.Method;
    client.Headers = params.Headers;
    client.JsonToArray = params.JsonToArray;
    client.PostData = params.PostData;
    client.MaxRedirects = params.MaxRedirects;
    client.Redirect = params.Redirect;
    setClient(id, client);

    return true;
};

int Clients::addClient(Arguments::Parameters params)
{
    Clients::Client client;
    client.MaxRedirects = params.MaxRedirects;
    client.Redirect = params.Redirect;
    client.PostData = params.PostData;
    client.Headers = params.Headers;
    client.JsonToArray = params.JsonToArray;
    client.Url = params.Url;
    client.Method = params.Url;
    
    int key = 1;
    while (true)
    {
        if (clients.find(key) == clients.end())
            break;
        key++;
    }

    clientsMtx.lock();
    clients[key] = client;
    clientsMtx.unlock();

    return key;
};

int Clients::AddClient(Output *op, Arguments::Parameters params)
{
    int id = addClient(params);
    if (id <= 0)
        return 101;
    
    op->Write(id);
    return 100;
};

int Clients::RemoveClient(Output *op, int id)
{
    op->Write(id);
    if (!removeClient(id))
        return 201;
    
    return 200;
};

int Clients::SetClient(Output *op, int id, Arguments::Parameters params)
{
    op->Write(id);
    if (!setClient(id, params))
        return 301;

    return 300;
};

bool Clients::removeClient(int id)
{
    std::map<int, Clients::Client>::iterator f;
    f = clients.find(id);

    if (f == clients.end())
        return false;
    
    clientsMtx.lock();
    clients.erase(f);
    clientsMtx.unlock();

    return true;
};
