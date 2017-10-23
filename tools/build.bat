
cd ..\
cl /MD /LD /Fe:arma3urlfetch.dll^
    src/Common.cpp^
    src/Requests.cpp^
    src/Clients.cpp^
    src/Output.cpp^
    src/Handler.cpp^
    src/json.hpp^
    src/main.cpp^
    /DCURL_STATICLIB^
    /I include\windows\x86^
    lib\windows\libcurl_a.lib
rm *.obj
rm *.lib
rm *.exp
exit