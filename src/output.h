
/*!
    \file Output.h
    \brief This file includes the Output class.
    \author Vincent Heins
    \see https://github.com/playnet-public/ArmA3URLFetch/blob/master/src/Output.cpp
*/

#pragma once
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*!
    \class Output
    \brief Is a simplified stringstream buffer.
*/
class Output
{
public:
    /*!
        \fn void Write(std::string *str)
        \brief Writes a std::string to Output::buf.
        \param str A std::string to write to Output::buf.
    */
    void Write(std::string *str);

    /*!
        \fn void Write(char *str)
        \brief Writes a char to Output::buf.
        \param str A char to write to Output::buf.
    */
    void Write(char *str);
    
    /*!
        \fn void Write(int id)
        \brief Writes an int to Output::buf.
        \param id An int to write to Output::buf.
    */
    void Write(int id);
    
    /*!
        \fn void Write(const char *str)
        \brief Writes a const char to Output::buf.
        \param str A constant char to write to Output::buf.
    */
    void Write(const char *str);
    
    /*!
        \fn void Flush()
        \brief Clears the buffer Output::buf.
    */
    void Flush();
    
    /*!
        \fn std::string Get()
        \brief Returns the current buffer as string.
        \return The current Output::buf content.
    */
    std::string Get();
    
    /*!
        \fn std::string GetFlush()
        \brief Same as Output::Get().
        \return The current Output::buf content.
        
        Cleares the buffer after copying the buffer.
    */
    std::string GetFlush();
    
    /*!
        \fn void WriteBuf(char *op, int oS)
        \brief Writes the current buffer to a given char pointer.
        \param op A pointer buffer to write to.
        \param oS The current buffer size of op.
    */
    void WriteBuf(char *op, int oS);
    
    /*!
        \fn void WriteBufFlush(char *op, int oS)
        \brief Same as Output::WriteBuf().
        \param op A pointer buffer to write to.
        \param oS The current buffer size of op.
        
        Cleares the buffer after copying to the char pointer.
    */
    void WriteBufFlush(char *op, int oS);
    
    /*!
        \fn static Output* Create()
        \brief Creates a new instance of Output.
        \return A newly created instance pointer of Output.
    */
    static Output* Create();
    
    /*!
        \fn static void Delete(Output *output)
        \brief Deletes a given instance from the memory.
        \param output A pointer to an Output.
    */
    static void Delete(Output *output);
private:
    std::stringstream buf; ///< The buffer itself.
};
