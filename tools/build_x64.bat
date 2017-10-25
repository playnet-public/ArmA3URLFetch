
cd ..\
cl /MD /LD /Fe:arma3urlfetch_x64.dll^
    src\Common.cpp^
    src\Requests.cpp^
    src\Clients.cpp^
    src\Output.cpp^
    src\Handler.cpp^
    src\main.cpp^
    /DCURL_STATICLIB^
    /I include\windows\x64^
    lib\windows\libcurl_a_x64.lib
rm *.obj
rm *.lib
rm *.exp
cd tools
