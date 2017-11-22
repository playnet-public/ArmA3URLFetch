
#include "Output.h"

//Output::Write writes an input parameter to the class instance specific buffer
void Output::Write(std::string * str)
{
    buf << str;
};

//---
void Output::Write(char *str)
{
    buf << str;
};

//---
void Output::Write(int n)
{
    buf << n;
};

//---
void Output::Write(const char *str)
{
    buf << str;
}

//Output::Flush clears the current buffer of the instantiated class
void Output::Flush()
{
    buf.flush();
};

//Output::Get returns the current buffer content
std::string Output::Get()
{
    return buf.str();
};

//Output::GetFlush same as Output::Get but flushes the buffer after returning content
std::string Output::GetFlush()
{
    std::string str = Get();
    Flush();
    return str;
};

//Output::WriteBuf Writes the full buffer content (including ending '\0') to a given char pointer
void Output::WriteBuf(char *op, int oS)
{
    buf << '\0';
    strncpy(op, buf.str().c_str(), oS);
};

//Output::WriteBufFlush does exactly the same as Output::WriteBuf but clears the buffer storage after returning the buffer content
void Output::WriteBufFlush(char *op, int oS)
{
    WriteBuf(op, oS);
    Flush();
};

//Output::Create creates and returns a new instance of this class
Output* Output::Create()
{
    return new Output();
};

//Output::Delete deletes the class instance given
void Output::Delete(Output *output)
{
    delete output;
};
