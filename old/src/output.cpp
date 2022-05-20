
#include "output.h"

void Output::Write(std::string * str)
{
    buf << str;
};

void Output::Write(char *str)
{
    buf << str;
};

void Output::Write(int n)
{
    buf << n;
};

void Output::Write(const char *str)
{
    buf << str;
}

void Output::Flush()
{
    buf.flush();
};

std::string Output::Get()
{
    return buf.str();
};

std::string Output::GetFlush()
{
    std::string str = Get();
    Flush();
    return str;
};

void Output::WriteBuf(char *op, int oS)
{
    buf << '\0';
    strncpy(op, buf.str().c_str(), oS);
};

void Output::WriteBufFlush(char *op, int oS)
{
    WriteBuf(op, oS);
    Flush();
};

Output* Output::Create()
{
    return new Output();
};

void Output::Delete(Output *output)
{
    delete output;
};
