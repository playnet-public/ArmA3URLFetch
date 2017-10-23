
#include "Common.h"
#include "Clients.h"
#include "Requests.h"
#include "Output.h"

class Handler {
public:
    Handler();
    Clients *clients;
    Requests *requests;
    int CallExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
private:
    std::map<std::string, std::string> parseArgs(const char **args, int argsCnt);
};
