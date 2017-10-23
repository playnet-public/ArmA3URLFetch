
#pragma once
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Output
{
public:
    void Write(std::string *str);
    void Write(char *str);
    void Write(int id);
    void Write(const char *str);
    void Flush();
    std::string Get();
    std::string GetFlush();
    void WriteBuf(char *op, int oS);
    void WriteBufFlush(char *op, int oS);
    static Output* Create();
    static void Delete(Output *output);
private:
    std::stringstream buf;
};
