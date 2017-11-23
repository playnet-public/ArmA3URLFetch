
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
        std::map<std::string, std::string> Parameters; ///< The parameters of a persistent URL request.
        std::vector<std::string> Headers; ///< The headers of a persistent URL request.
    };

    /*!
        \fn int AddClient(Output *op, std::map<std::string, std::string> params)
        \brief The public function to add a client.
        \param op A pointer to an output stream.
        \param params The map of parameters for the client.
        \return The error code of the request.
        
        This function calls Clients::addClient but writes the result to an output.
    */
    int AddClient(Output *op, std::map<std::string, std::string> params);

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
    int SetParameters(Output *op, int id, std::map<std::string, std::string> params);

    /*!
        \fn int SetHeaders(Output *op, int id, std::vector<std::string> headers)
        \brief The public function to set sepcific client headers.
        \param op A pointer to an output stream.
        \param id A client unique id.
        \param headers The map of headers for the client.
        \return The error code of the request.
    */
    int SetHeaders(Output *op, int id, std::vector<std::string> headers);

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
        \fn int addClient(std::map<std::string, std::string> params)
        \brief Adds a client to Clients::clients.
    */
    int addClient(std::map<std::string, std::string> params);

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
        \fn bool setParameters(int id, std::map<std::string, std::string> params)
        \brief Sets the parameters of a client by its id 
    */
    bool setParameters(int id, std::map<std::string, std::string> params);

    /*!
        \fn bool setHeaders(int id, std::vector<std::string> headers)
        \brief Sets the headers of a client by its id
    */
    bool setHeaders(int id, std::vector<std::string> headers);
};
