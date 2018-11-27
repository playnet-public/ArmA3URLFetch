
/*!
    \file Requests.h
    \brief Contains the class Requests.
    \author Vincent Heins
    \see https://github.com/playnet-public/ArmA3URLFetch/blob/master/src/Requests.cpp
*/

#pragma once
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <curl/curl.h>
#include "arguments.h"
#include <stdio.h>
#include <stdlib.h>
#include "output.h"
#include "macros.h"
#include "common.h"

/*!
    \class Requests
    \brief This class is used to process given requests.
*/
class Requests
{
public:
    #ifdef __linux__
    /*!
        \fn Requests()
        \brief Is the initialization of Requests
    */
    Requests();
    #endif

    /*!
        \struct Result
        \brief This struct contains the end-status and result of a request.
    */
    struct Result
    {
        int status; ///< Status of the result.
        std::string result; ///< URL content of the URL request.
    };

    /*!
        \struct Request
        \brief This struct contains the information needed for a request.
    */
    struct Request
    {
        int RequestID; ///< Unique ID of the request.
        int MaxRedirects;
        bool JsonToArray, Redirect;
        std::string Url, Method, Forms, PostData; ///< Parameters of the request.
        std::vector<std::string> Headers; ///< Headers of the request.
    };

    /*!
        \fn int AddRequest(Output *op, std::map<std::string, std::string> params)
        \brief Adds an request with parameters.
        \param op A pointer to an output stream.
        \param params The struct of Arguments::Parameters for the request.
        \return The error code of the request.
        
        Writes to an Output buffer.
    */
    int AddRequest(Output *op, Arguments::Parameters params);

    /*!
        \fn int GetResult(Output *op, int id)
        \brief Returns an result of a given id.
        \param op A pointer to an output stream.
        \param id The unique id of the result.
        \return The error code of the request.
        
        Writes to an Output buffer.
    */
    int GetResult(Output *op, int id);

    int GetStatus(int id);
private:
    std::map<int, Requests::Result> results; ///< The list of all pending results/requests.
    std::mutex resultsMtx; ///< The list mutex.
    std::queue<Requests::Request> requestsQueue; ///< The queue for all requests.
    std::mutex requestsQueueMtx; ///< The mutex of the queue.
    bool workersStarted = false; ///< The lock for a initialization once a time.

    /*!
        \fn void getPopRequest(Requests::Request *req)
        \brief Copies the first request from the queue and removes it from the queue.
    */
    void getPopRequest(Requests::Request *req);

    /*!
        \fn void startWorkers()
        \brief Setups all workers and set the lock Requests::workersStarted
    */
    void startWorkers();

    /*!
        \fn void fetchRequest(Requests::Request req)
        \brief Processes a request. Is called by Requests::workerThread().
    */
    void fetchRequest(Requests::Request req);

    /*!
        \fn bool isValidMethod(std::string method)
        \brief Checks if the given method exists.
    */
    bool isValidMethod(std::string method);

    /*!
        \fn bool isValidParameter(std::string param)
        \brief Checks the validity of the given parameter
    */
    bool isValidParameter(std::string param);
    
    /*!
        \fn void workerThread()
        \brief This is a worker thread function.
    */
    void workerThread();

    /*!
        \fn void addRequest(std::map<std::string, std::string> params)
        \brief Adds a Requests::Request with parameters to the Requests::requestsQueue.
    */
    int addRequest(Arguments::Parameters params);

    /*!
        \fn int addResult()
        \brief Adds an empty struct Requests::Result to the list Requests::results.
    */
    int addResult();

    /*!
        \fn void setResult(int id, Requests::Result res)
        \brief Sets a specifc result available in Requests::results by id.
    */
    void setResult(int id, Requests::Result res);
    
    /*!
        \fn bool removeResult(int id)
        \brief Removes a list entry of Requests::results.
    */
    bool removeResult(int id);

    /*!
        \fn int getResult(int id, Requests::Result *req)
        \brief Copies a specific struct Requests::Result to a given pointer.
    */
    int getResult(int id, Requests::Result *req); //0, 1, 2
    
    /*!
        \fn int getResultString(int id, std::string *str)
        \brief Copies only the result from a specific Requests::Result to a given pointer.
    */
    int getResultString(int id, std::string *str);
};
