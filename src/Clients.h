
/*!
    \file Clients.h
    \brief Clients is a class for persistent requests.
*/

#pragma once
#include <map>
#include <vector>
#include <mutex>
#include "Output.h"

/*!
    \class Clients
    \brief Clients is a class for persistent requests.
*/

class Clients
{
public:
    /*!
        \struct Client
        \memberof Clients
        \brief The default Client struct.

        The client struct contains information such as parameters and its url headers for persistent calling.
    */
    struct Client
    {
        std::map<std::string, std::string> Parameters;
        std::vector<std::string> Headers;
    };

    /*!
        \fn int AddClient(Output *op, std::map<std::string, std::string> params)
        \brief The public function to add a client.
        
        This function calls \link Clients::addClient \endlink but writes the result to an output.
    */
    int AddClient(Output *op, std::map<std::string, std::string> params);

    /*!
        \fn int RemoveClient(Output *op, int id)
        \brief The public function to remove a client.

        This function calls Clients::removeClient but writes the result to an output.
    */
    int RemoveClient(Output *op, int id);

    /*!
        \fn int SetParameters(Output *op, int id, std::map<std::string, std::string> params)
        \brief The public function to set specific client parameters.
    */
    int SetParameters(Output *op, int id, std::map<std::string, std::string> params);

    /*!
        \fn int SetHeaders(Output *op, int id, std::vector<std::string> headers)
        \brief The public function to set sepcific client headers.
    */
    int SetHeaders(Output *op, int id, std::vector<std::string> headers);

    //Clients::GetClient returns a client by a given id

    /*!
        \fn bool GetClient(int id, Clients::Client *client)
        \brief The public function to gather a specific \link Clients::Client \endlink
    */
    bool GetClient(int id, Clients::Client *client);
private:
    std::map<int, Clients::Client> clients; ///< Contains available clients.
    std::mutex clientsMtx;
    int addClient(std::map<std::string, std::string> params);
    bool removeClient(int id);

    //Clients::setClient sets a client struct by given id and Clients::Client struct
    void setClient(int id, Clients::Client cli);
    bool setParameters(int id, std::map<std::string, std::string> params);
    bool setHeaders(int id, std::vector<std::string> headers);
};
