
/*!
    \file Clients.h
    \brief Clients is a class for persistent requests.
    \author Vincent Heins
    \see https://github.com/playnet-public/ArmA3URLFetch/blob/master/src/Clients.cpp
*/

#pragma once
#include <map>
#include <vector>
#include <mutex>
#include "output.h"
#include "arguments.h"

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
        bool JsonToArray;
        std::string Url, Method, PostData; ///< The parameters of a persistent URL request.
        std::vector<std::string> Headers; ///< The headers of a persistent URL request.
    };

    /*!
        \fn int AddClient(Output *op, Handler::Parameters params)
        \brief The public function to add a client.
        \param op A pointer to an output stream.
        \param params The Handler::Parameters of parameters for the client.
        \return The error code of the request.
        
        This function calls Clients::addClient but writes the result to an output.
    */
    int AddClient(Output *op, Arguments::Parameters params);

    /*!
        \fn int RemoveClient(Output *op, int id)
        \brief The public function to remove a client.
        \param op A pointer to an output stream.
        \param id A client unique id.
        \return The error code of the request.

        This function calls Clients::removeClient but writes the result to an output.
    */
    int RemoveClient(Output *op, int id);

    /*!
        \fn int SetParameters(Output *op, int id, std::map<std::string, std::string> params)
        \brief The public function to set specific client parameters.
        \param op A pointer to an output stream.
        \param id A client unique id.
        \param params The map of parameters for the client.
        \return The error code of the request.
    */
    int SetClient(Output *op, int id, Arguments::Parameters params);

    /*!
        \fn bool GetClient(int id, Clients::Client *client)
        \brief The public function to gather a specific Clients::Client.
        \param id A client unique id.
        \param client A client pointer to copy to.
        \return Was the client requested successful or not.
    */
    bool GetClient(int id, Clients::Client *client);
private:
    std::map<int, Clients::Client> clients; ///< Contains available clients.
    std::mutex clientsMtx; ///< The mutex for the available clients Clients::clients.

    /*!
        \fn int addClient(Arguments::Parameters params)
        \brief Adds a client to Clients::clients.
    */
    int addClient(Arguments::Parameters params);

    /*!
        \fn bool removeClient(int id)
        \brief Removes a client from Clients::clients.
    */
    bool removeClient(int id);

    /*!
        \fn void setClient(int id, Clients::Client cli)
        \brief Sets a Clients::Client of Clients::clients by given id and Clients::Client.
    */
    void setClient(int id, Clients::Client cli);

    /*!
        \fn bool setClient(int id, Arguments::Parameters params)
        \brief Sets the parameters of a client by its id 
    */
    bool setClient(int id, Arguments::Parameters params);
};
