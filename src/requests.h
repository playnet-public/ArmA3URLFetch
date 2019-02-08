
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
#include <condition_variable>
#include <atomic>

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
        Result(int s) {
            status = s;
        }
        ~Result() {
            result.clear();
        }
        int status; ///< Status of the result.
        long httpCode;
        std::string result; ///< URL content of the URL request.
    };

    /*!
        \struct Request
        \brief This struct contains the information needed for a request.
    */
    struct Request
    {
        Request(int id,
            int maxRedirects,
            long maxTimeout,
            bool jsonToArray,
            bool redirect,
            std::string url,
            std::string method,
            std::string postData,
            std::vector<std::string> headers) {
                RequestID = id;
                MaxRedirects = maxRedirects;
                MaxTimeout = maxTimeout;
                JsonToArray = jsonToArray;
                Redirect = redirect;
                Url = url;
                Method = method;
                PostData = postData;
                Headers = headers;
        }
        ~Request() {
            Url.clear();
            Method.clear();
            PostData.clear();
            Headers.clear();
        }
        int RequestID; ///< Unique ID of the request.
        int MaxRedirects;
        long MaxTimeout;
        bool JsonToArray, Redirect;
        std::string Url, Method, PostData; ///< Parameters of the request.
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
    std::queue<Requests::Request*> requests; ///< The queue for all requests.
    std::mutex m_requests; ///< The mutex of the queue.
    std::condition_variable cv_requests;

    std::map<int, Requests::Result*> results; ///< The list of all pending results/requests.
    std::mutex m_results; ///< The list mutex.

    std::atomic_int lastID;

    bool workersStarted = false; ///< The lock for a initialization once a time.

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
// 0 = text pending, 1 = pending, 2 = error
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
        \fn bool removeResult(int id)
        \brief Removes a list entry of Requests::results.
    */
    bool removeResult(int id);

    /*!
        \fn Requests::Result* getResult(int id)
        \brief Copies a specific struct Requests::Result to a given pointer.
    */
    Requests::Result* getResult(int id); //0, 1, 2
    
    /*!
        \fn int getResultString(int id, std::string *str)
        \brief Copies only the result from a specific Requests::Result to a given pointer.
    */
    int getResultString(int id, std::string *str);
};
