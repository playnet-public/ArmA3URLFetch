
/*!
    \file handler.h
    \brief Handler is the main class for input parameters of RVExtensionArgs().
    \see https://github.com/playnet-public/ArmA3URLFetch/blob/master/src/Handler.cpp
*/

#pragma once
#include "common.h"
#include "requests.h"
#include "clients.h"
#include "output.h"

/*!
    \class Handler
    \brief Handler handles the input parameters of RVExtensionArgs().
*/
class Handler {
public:
    /*!
        \fn Handler()
        \brief This is the initialization of Handler.
    */
    Handler();
    
    Clients *clients; ///< The main Clients class instance.
    Requests *requests; ///< The main Requests class instance.

    /*!
        \fn int CallExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt)
        \brief This is just a wrapper for the exported RVExtensionArgs() function.
        \param output The buffer pointer of ArmA 3.
        \param outputSize The current buffersize returned by ArmA 3.
        \param function The transmitted function to call.
        \param args The additional arguments of the ArmA 3 call.
        \param argsCnt The amount of arguments available in args.
        \return A custom error code for ArmA 3.
    */
    int CallExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
private:
    /*!
        \fn std::map<std::string, std::string> parseArgs(const char **args, int argsCnt)
        \brief Converts args pointer array of RVExtensionArgs() to a string map.
    */
    int sendRequest(Output *op, const char **args, int argsCnt);
    int addClient(Output *op, const char **args, int argsCnt);
    int removeClient(Output *op, const char **args, int argsCnt);
    int setClient(Output *op, const char **args, int argsCnt);
    int getRequest(Output *op, const char **args, int argsCnt);
    int getStatus(Output *op, const char **args, int argsCnt);
};
